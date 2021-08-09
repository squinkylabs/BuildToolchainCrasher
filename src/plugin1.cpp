/**
 * This file contains the entire implementation of the VCO1 demo module.
 * Although it is possible to implement more than one module in a single file,
 * it is rarely done.
 */

#include "global-plugin.hpp"

#include <random>

#include "rack.hpp"
 


/**
 *  Every synth module must have a Module structure.
 *  This is where all the real-time processing code goes.
 */
struct VCO1Module : Module
{
    enum ParamIds {
        NUM_PARAMS
	};
	enum InputIds {
        NUM_INPUTS
	};
	enum OutputIds {
        OUTPUT,
        NUM_OUTPUTS
	};
	enum LightIds {
        NUM_LIGHTS
    };

    VCO1Module() {
        // Your module must call config from its constructor, passing in
        // how many ins, outs, etc... it has.
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    }

    // Every Module has a process function. This is called once every
    // sample, and must service all the inputs and outputs of the module.
    void process(const ProcessArgs& args) override {
        const size_t size = 100000;
        int testArray[size];
        float f = gen();
        outputs[OUTPUT].setVoltage(f);
        if (count == 4000) {
            INFO("proces... f=%f", f);
            INFO("sum = %d", sum);
            count = 0;
        } else {
            count++;
        }

        static bool init = false;
        if (!init) {
            srand(1234);
            init = true;
        }
        for (size_t i=0; i<size; ++i) {
            testArray[i] = rand();
        }
        for (size_t i=0; i<size; ++i) {
            testArray[i] += rand();
        }
        int sum = 0;
        for (size_t i=0; i<size; ++i) {
            sum += testArray[i];
        }
        

    }

    std::mt19937 gen;
    int count = 0;
    int sum = 0;


};

/**
 * At least in VCV 1.0, every module must have a Widget, too.
 * The widget provides the user interface for a module.
 * Widgets may draw to the screen, get mouse and keyboard input, etc...
 * Widgets cannot actually process or generate audio.
 */
struct VCO1Widget : ModuleWidget {
    VCO1Widget(VCO1Module* module) {
        // The widget always retains a reference to the module.
        // you must call this function first in your widget constructor.
        setModule(module);

        // Typically the panel graphic is added first, then the other 
        // UI elements are placed on TOP.
        // In VCV the Z-order of added children is such that later
        // children are always higher than children added earlier.
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/panel.svg")));

        // VCV modules usually have image is "screws" to make them
        // look more like physical module. You may design your own screws, 
        // or not use screws at all.
		addChild(createWidget<ScrewSilver>(Vec(15, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 0)));
		addChild(createWidget<ScrewSilver>(Vec(15, 365)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 365)));

        // Add some quick hack labels to the panel.
        addLabel(Vec(20, 20), "Demo Crasher");

    }

    // Simple helper function to add test labels to the panel.
    // In a real module you would draw this on the panel itself.
    // Labels are fine for hacking, but they are discouraged for real use.
    // Some of the problems are that they don't draw particularly efficiently,
    // and they don't give as much control as putting them into the panel SVG.
    Label* addLabel(const Vec& v, const std::string& str)
    {
        NVGcolor black = nvgRGB(0,0,0);
        Label* label = new Label();
        label->box.pos = v;
        label->text = str;
        label->color = black;
        addChild(label);
        return label;
    }
};


Model* crasherPlugin1 = createModel<VCO1Module, VCO1Widget>("crasher-plugin1");