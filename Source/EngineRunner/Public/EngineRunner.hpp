#pragma once

#include <Configuration.hpp>

class externalWindowInterface;
class renderContextInterface;
class inputSourceInterface;
class engine;
class icomponentRepository;

class engineRunner 
{
public:
    engineRunner(externalWindowInterface * p_window_ptr, renderContextInterface * p_render_context_ptr, inputSourceInterface * p_input_source_ptr);
    virtual ~engineRunner();

    void initialize(configuration p_config);
    void update();
    void render();
    void shutdown();

    engine * getEngine() { return m_engine; }

private:
    icomponentRepository * m_component_repo;
    engine * m_engine;
};

