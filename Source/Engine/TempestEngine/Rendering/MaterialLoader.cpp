#include "MaterialLoader.hpp"
#include "Logger.hpp"

materialData::materialData(std::wstring p_type, material p_mat)
	: assetData{std::move(p_type)}
	, m_mat{ std::move(p_mat) }
{
}

material const & materialData::getData() const noexcept
{
	return m_mat;
}

void materialLoader::assetsFrom(const std::filesystem::path & p_base, const std::wstring & p_filename, std::vector<asset*>& p_assets)
{
	p_assets.emplace_back(new asset(p_filename, std::filesystem::relative(p_filename, p_base).generic_wstring(), this));
}

void materialLoader::loadAsset(asset & p_asset)
{
	using setter = void(*)(material&, std::stringstream&);

	static const std::unordered_map<std::string_view, setter> set
	{
		{"newmtl", +[](material& m, std::stringstream & l_ss)
			{
				std::string str; l_ss >> str;
				m.setMaterialName(str);
			}
		},

		{"illum" , +[](material& m, std::stringstream & l_ss)
			{
				int model;
				l_ss >> model;
				m.setIlluminationModel(model);
			}
		},

		{"Ka", +[](material& m, std::stringstream & l_ss)
			{
				color l_color;
				auto&&[red, green, blue, alpha] = l_color;
				l_ss >> red >> green >> blue; alpha = 1.0f;
				m.setAmbientColor(l_color);
			}
		},
		{"Kd", +[](material& m, std::stringstream & l_ss)
			{
				color l_color;
				auto&&[red, green, blue, alpha] = l_color;
				l_ss >> red >> green >> blue; alpha = 1.0f;
				m.setDiffuseColor(l_color);
			}
		},
		{"Ks", +[](material& m, std::stringstream & l_ss)
			{
				color l_color;
				auto&&[red, green, blue, alpha] = l_color;
				l_ss >> red >> green >> blue; alpha = 1.0f;
				m.setSpecularColor(l_color);
			}
		},
		{"Ke", +[](material& m, std::stringstream & l_ss)
			{
				color l_color;
				auto&&[red, green, blue, alpha] = l_color;
				l_ss >> red >> green >> blue; alpha = 1.0f;
				m.setEmissiveColor(l_color);
			}
		},
		{"Ns", +[](material& m, std::stringstream & l_ss)
			{
				float spec; l_ss >> spec;
				m.setSpecularExponent(spec);
			}
		},
		{"map_Kd", +[](material& m, std::stringstream & l_ss)
			{
				std::string str; l_ss >> str;
				m.setDiffuseMapName(str);
			}
		},
		{"map_Ks", +[](material& m, std::stringstream & l_ss)
			{
				std::string str; l_ss >> str;
				m.setSpecularMapName(str);
			}
		},
		{"map_Kn", +[](material& m, std::stringstream & l_ss)
			{
				std::string str; l_ss >> str;
				m.setNormalMapName(str);
			}
		},
	};
	
	std::ifstream ss{ p_asset.getFullPath() };

	if (!ss)
	{
		logger("MaterialLoader").error() << "failed to load " << p_asset.getFullPath().u8string() << std::endl;
		return;
	}

	std::string l_line;
	std::string word;

	material mat;

	while (std::getline(ss, l_line))
	{
		if (*l_line.c_str() == '#')
			continue;

		if (l_line.length() == 0)
			continue;

		std::stringstream  l_stringstream;
		l_stringstream << std::move(l_line);
		l_stringstream >> word;

		if (const auto found = set.find(word); found != set.cend())
		{
			found->second(mat, l_stringstream);
		}

		word.clear();
	}
	mat.m_isLoaded = true;

	p_asset.setData(new materialData{ p_asset.getExtention(), std::move(mat) });
}

void materialLoader::unloadAsset(asset & p_asset)
{
	delete &p_asset.getData<materialData>();
	p_asset.setData(nullptr);
}
