#include "ShaderLoader.hpp"
#include "DataTypes.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <optional>
#include <system_error>
#include <functional>
#include <charconv>

using str_view = std::string_view;

template<typename T>
auto from_chars(str_view token, T& val)
{
	return std::from_chars(token.data(), token.data() + token.length(), val);
}

template<size_t size, typename T>
auto to_chars(std::array<char, size>& buffer, T const& val)
{
	static_assert(size >= std::numeric_limits<T>::digits10, "buffer potentially too small");
	return std::to_chars(buffer.data(), buffer.data() + buffer.size(), val);
}

constexpr auto findStringLiteral(const str_view view) noexcept
	-> std::optional<str_view>
{
	const auto begin = view.find('\"');
	if (begin == str_view::npos) return std::nullopt;
	const auto end = view.find('\"', begin + 1);
	if (end == str_view::npos) return std::nullopt;
	return view.substr(begin + 1, end - begin - 1);
}



constexpr auto getNextToken(str_view& line) noexcept
	-> std::optional<str_view>
{
	const auto begin = line.find_first_not_of(" \t");
	const auto end = line.find_first_of(" \t", begin);

	if (begin == line.npos) return std::nullopt;

	if (end == line.npos)
	{
		line.remove_prefix(begin);
		str_view token;
		line.swap(token);
		return { token };
	}

	const auto token = line.substr(begin, end - begin);

	line.remove_prefix(end);

	return { token };
}

constexpr auto findPreprocessorToken(const str_view view, const str_view token) noexcept
	-> std::optional<str_view>
{
	const auto begin = view.find('#');

	if (begin == view.npos) return std::nullopt;

	if (const auto found = view.substr(begin + 1, token.size()); found == token)
	{
		return view.substr(begin + 1 + token.size());
	}

	return std::nullopt;
}

bool stripComments(std::string& str)
{
	constexpr auto commonStart = '/';
	
	constexpr auto cStyle = '*';
	constexpr auto cend = "*/";
	constexpr auto cReplace = ' ';

	constexpr auto cppend = '\n';
	constexpr auto cppStyle = '/';
	constexpr auto cppReplace = '\n';


	size_t out{ 0 };
	str_view view{ str };

	// unstructured looping. Loop until we reach the end of the file
	for (;;)
	{
		// find a potential comment beginning
		const auto found = view.find(commonStart);

		// base case: no more comments!
		if (found == view.npos)
		{
			// copy the rest of the string to the info
			const auto copied = view.copy(str.data() + out, view.length());
			str.resize(out + copied);
			return true;
		}
		else // check if we really found a comment
		{
			const auto commentRemoval = 
			[&str, &out, &view, &found](const auto replace, const str_view endTok) -> bool
			{
				// replace comment with space
				str[out++] = replace;

				// skip past the two-character sequence
				view.remove_prefix(found + 2);

				const auto end = view.find(endTok);

				// unclosed comment
				if (end == view.npos)
				{
					str.resize(out);
					return false;
				}

				view.remove_prefix(end + endTok.length());
				return true;
			};

			// copy up-to the slash
			out += view.copy(str.data() + out, found);

			switch (view[found + 1])
			{
			case cStyle: {
				if (!commentRemoval(cReplace, cend))
					return false;
			} break;

			case cppStyle: {
				if (!commentRemoval(cppReplace, str_view{ &cppend, 1 }))
					return true;
			} break;

			default: {
				str[out++] = view[found];
				// skip past the slash and continue
				view.remove_prefix(found + 1);
			}break;
			}
		}
	}
}

constexpr str_view getLine(str_view& view) noexcept
{
	// find the next newline
	const auto next = view.find('\n');

	const auto line{ view.substr(0, next) };

	if (next == view.npos)
	{
		view.remove_prefix(view.length());
	}
	else
	{
		view.remove_prefix(line.length() + 1);
	}

	return line;
}


void shaderLoader::assetsFrom(const std::filesystem::path& p_base, const std::wstring& p_filename, std::vector<asset*>& p_assets)
{
	p_assets.emplace_back(new asset(p_filename, std::filesystem::relative(p_filename, p_base).generic_wstring(), this));
}

enum class loadingErrors
{
	noError = 0,

	// generic
	fileDoesntExist,
	failedToLoad,
	badToken,

	// include errors
	badInclude,
	nonincludable,
	selfDependency,
	circularDependency,

	// preprocessor errors

	// pragma
	badPragma,
	pragmaNoArgs,
	pragmaExpectedLiteral,

	// comment errors
	unclosedComment,
};

struct getSourceReturn
{
	loadingErrors error;
	std::string source;
	std::vector<assetHandle> depends;
};

getSourceReturn getSourceAndDependencies(std::ifstream& file, assetHandle p_this, str_view p_name)
{
	const auto [success, rawData] =
	[](std::ifstream& file) -> std::pair<bool, std::string>
	{
		std::stringstream ss;
		ss << file.rdbuf();
		file.close();
		auto str = ss.str();
		const bool success = stripComments(str);
		return std::make_pair(success, std::move(str));
	}(file);

	if (!success)
	{
		return { loadingErrors::unclosedComment };
	}

	std::set<asset*> l_depends;
	std::vector<assetHandle> l_code_emit_order;

	str_view view{ rawData };

	std::string source; source.reserve(rawData.size());



	constexpr str_view preprocessorTokens[] =
	{
		"version"
		"include",
		"pragma"
	};




	using handlerReturn = std::pair<loadingErrors, str_view>;
	struct shaderContext
	{
		std::string& output;
		str_view& leftToProcess;
		str_view line;

		str_view name;
		std::set<asset*>& depends;
		std::vector<assetHandle>& emitOrder;
	};

	using handler =  handlerReturn(*)(shaderContext const& context, str_view line);

	// consumes line without modifying output
	constexpr handler doNothing = +[](shaderContext const&, str_view) -> handlerReturn { return { loadingErrors::noError, {} }; };

	constexpr handler handleVersion = doNothing; // TODO: maybe write it down somewhere?

	constexpr handler handleInclude = 
		+[](shaderContext const& context, str_view toParse) -> handlerReturn
	{
		const auto include = findStringLiteral(toParse);
		if (!include)
		{
			return { loadingErrors::badInclude, toParse };
		}

		// self-dependency check
		if (include == toParse)
		{
			return { loadingErrors::selfDependency, {} };
		}

		// get the file from the asset manager
		const auto depend = assetManager::getAsset(
			std::filesystem::path{ "shaders" } / "include" / *include
		);

		// file existence check
		if (!depend)
		{
			return { loadingErrors::fileDoesntExist, *include };
		}

		// potential recursion (if not already loaded)
		auto* const ptr = depend.get();

		// successful load check (if one file fails to load, all of them will)

		if (!ptr)
		{
			return { loadingErrors::failedToLoad, *include };
		}

		auto const& data = depend->getData<shaderData>();

		// valid include check
		if (data.getShaderType() != shaderType::enm_glsl)
		{
			return { loadingErrors::nonincludable, *include };
		}

		// if we haven't dealt with this dependency yet
		if (context.depends.emplace(ptr).second)
		{
			// insert their dependencies first
			for (auto&& theirDepend : data.getDependencies())
			{
				auto* const theirPtr = theirDepend.get();

				if (theirPtr == ptr)
				{
					return { loadingErrors::circularDependency, *include };
				}

				if (context.depends.emplace(theirPtr).second)
				{
					context.emitOrder.emplace_back(theirDepend);
				}
			}

			// insert my dependency
			context.emitOrder.emplace_back(depend);
		}

		return { loadingErrors::noError, {} };
	};

	constexpr handler handlePragma = 
		+[](shaderContext const& context, str_view toParse) -> handlerReturn
	{
		// string view starts here
		//        v
		// #pragma TempestKernelLooping <callable> <integer literal>

		// hard coded to just this, for now. 
		// Need secondary lookups on the first token
		constexpr auto pragma = str_view{ "TempestKernelLooping" };
		
		size_t count = 0;
		std::array<str_view, 3> expectedTokens;

		for (; count != 3; ++count)
		{
			const auto token = getNextToken(toParse);

			if (!token) return { loadingErrors::badToken, context.line };

			expectedTokens[count] = *token;
		}

		// pragma with no parameters
		if (count == 0) return { loadingErrors::badPragma, {} }; 
		if (expectedTokens[0] != pragma)
		{
			// forward to GLSL compiler
			context.output.append(context.line);
			return { loadingErrors::noError, {} };
		}
		
		// we handle this pragma, but we didn't receive enough args
		if (count < expectedTokens.max_size()) 
			return { loadingErrors::pragmaNoArgs, context.line };

		const auto& callable   = expectedTokens[1];
		const auto& intLiteral = expectedTokens[2];
		
		// try to convert to int on the spot
		std::int64_t val;
		const auto convErr = from_chars(intLiteral, val);

		static constexpr auto appendAsStr = [](std::string& str, const auto val)
		{
			using T = decltype(val);
			using limits = std::numeric_limits<T>;
			std::array<char, limits::digits10 + 1> buffer;

			const auto conv = to_chars(buffer, val);
			if (conv.ec == std::errc{})
			{
				str.append(buffer.data(), conv.ptr);
			}
			return conv.ec;
		};

		const auto genLoop = [&context, callable](const auto val)
		{
			// find the extents of the bounding box we want to sample
			const auto bound = std::abs(val / 2);

			// manually unroll the GLSL loop with this rolled loop :p
			for (auto i = -bound;;)
			{
				for (auto j = -bound;;)
				{
					// creates a fold expression of all n^2 terms being summed (first term is positive :) )
					// for each i and j pair
					// +callable(0,0) +callable(0,1) ... +callable(0,j) ... +callable(i,j)
					context.output.append(callable);


					context.output.push_back('(');

					appendAsStr(context.output, i);

					context.output.push_back(',');

					appendAsStr(context.output, j);

					context.output.append(")\n");

					if (j == bound) break;

					context.output.push_back('+');
					++j;
				}

				if (i == bound) break;

				context.output.push_back('+');
				++i;
			}
		};

		if (convErr.ec == std::errc{})
		{
			genLoop(val);
			return { loadingErrors::noError, {} };
		}

		return { loadingErrors::pragmaExpectedLiteral, intLiteral };
	};

	while (!view.empty())
	{
		// get the next line
		const auto line = getLine(view);

		const auto context = shaderContext{ source, view, line, p_name, l_depends, l_code_emit_order };

		if (const auto versioning = findPreprocessorToken(line, "version"))
		{
			// strip versioning information
			// TODO: Maybe mark this down somewhere?
			continue;
		}

		const auto including = findPreprocessorToken(line, "include");
		if(including)
		{
			auto error = handleInclude(context, *including);
			if (error.first != loadingErrors::noError)
				return { error.first, std::string{error.second}, {} };
			continue;
		}

		const auto pragma = findPreprocessorToken(line, "pragma");
		if (pragma)
		{
			auto error = handlePragma(context, *pragma);
			if (error.first != loadingErrors::noError)
				return { error.first, std::string{error.second}, {} };
			continue;
		}

		source.append(line);
		source.push_back('\n');
	}

	return { loadingErrors::noError, std::move(source), std::move(l_code_emit_order) };
}

void logLoadingErrors(asset& p_asset, const loadingErrors error, std::string&& info)
{
	auto log = logger{ "shaderLoader" }.error();
	log << "In \"" << p_asset.getFullPath().generic_string() << "\":\n\t";
	switch (error)
	{
	case loadingErrors::circularDependency: {
		log << '"' << std::move(info) << "\" created a circular dependency";
	}break;

	case loadingErrors::failedToLoad: {
		log << "Failed to load dependency \"" << std::move(info) << '"';
	} break;

	case loadingErrors::nonincludable: {
		log << "File depends on \"" << std::move(info) << "\", a non-.glsl file (unincludable)";
	}break;

	case loadingErrors::selfDependency: {
		log << "This file includes itself directly";
	} break;

	case loadingErrors::unclosedComment: {
		log << "This file had an unclosed multiline comment";
	}break;

	case loadingErrors::fileDoesntExist: {
		log << '"' << std::move(info) << "\" doesn't exist or is inaccessible";
	}break;

	case loadingErrors::badInclude: {
		log << "Failed to parse include directive:\n\t\""
			<< std::move(info) << '"';
	}break;

	default: {
		log << "something *very* bad happened";
	}break;
	}
}

void shaderLoader::loadAsset(asset& p_asset)
{
	const auto l_type = 
	[](const auto& l_ext) noexcept -> shaderType
	{
		if (l_ext == L".vert") return shaderType::enm_vert;
		if (l_ext == L".frag") return shaderType::enm_frag;
		if (l_ext == L".geom") return shaderType::enm_geom;
		if (l_ext == L".tesc") return shaderType::enm_tesc;
		if (l_ext == L".tese") return shaderType::enm_tese;
		if (l_ext == L".glsl") return shaderType::enm_glsl;
		return shaderType::enm_none; // screw you, me! I'm using this explicitly!!!!
	}(p_asset.getExtention());

	auto getName = [&p_asset]()
	{
		return p_asset.getFullPath().generic_string();
	};
	
	if (l_type == shaderType::enm_none)
	{
		logger("shaderLoader").error() << "Unknown shadertype for \"" << getName() << '"';
		return;
	}

	std::ifstream file(p_asset.getFullPath());

	if (!file)
	{
		logger("shaderLoader").error() << "Could not find file \"" << getName() << '"';
		return;
	}

	auto&& [error, source, depends] = getSourceAndDependencies(file, assetHandle{ &p_asset }, getName());
	
	if (error != loadingErrors::noError)
	{
		logLoadingErrors(p_asset, error, std::move(source));
		return;
	}

	p_asset.setData(new shaderData{ l_type, std::move(source), std::move(depends)});
}

void shaderLoader::unloadAsset(asset& p_asset)
{
	delete &p_asset.getData<shaderData>();
	p_asset.setData(nullptr);
}
