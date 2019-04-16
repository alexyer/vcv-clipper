#include <algorithm>
#include "Template.hpp"


struct MyModule : Module {
	enum ParamIds {
		DRIVE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		DRIVE_INPUT,
		DRIVE_CV_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		DRIVE_OUTPUT,
		NUM_OUTPUTS
	};

	float threshold = 0.0f;

	MyModule() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};


void MyModule::step() {
	threshold = params[DRIVE_PARAM].value;
	float drive = threshold + inputs[DRIVE_CV_INPUT].value;
	float frame = inputs[DRIVE_INPUT].value;

	if (frame <= 0.0) {
	    outputs[DRIVE_OUTPUT].value = std::max(frame, drive) / drive;
	} else {
        outputs[DRIVE_OUTPUT].value = std::min(frame, -drive) / drive;
	}
}


struct MyModuleWidget : ModuleWidget {
	MyModuleWidget(MyModule *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/MyModule.svg")));

		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(28, 87), module, MyModule::DRIVE_PARAM, -5.0, -0.01, 0.0));

		addInput(Port::create<PJ301MPort>(Vec(33, 186), Port::INPUT, module, MyModule::DRIVE_INPUT));
		addInput(Port::create<PJ301MPort>(Vec(33, 230), Port::INPUT, module, MyModule::DRIVE_CV_INPUT));

		addOutput(Port::create<PJ301MPort>(Vec(33, 275), Port::OUTPUT, module, MyModule::DRIVE_OUTPUT));
	}
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelMyModule = Model::create<MyModule, MyModuleWidget>("Olexander Yermakov", "clipper", "clipper", DISTORTION_TAG);
