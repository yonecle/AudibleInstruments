#include "AudibleInstruments.hpp"


struct Links : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		A1_INPUT,
		B1_INPUT,
		B2_INPUT,
		C1_INPUT,
		C2_INPUT,
		C3_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		A1_OUTPUT,
		A2_OUTPUT,
		A3_OUTPUT,
		B1_OUTPUT,
		B2_OUTPUT,
		C1_OUTPUT,
		NUM_OUTPUTS
	};

	float lights[3] = {};
	Links();
	void step();
};


Links::Links() {
	params.resize(NUM_PARAMS);
	inputs.resize(NUM_INPUTS);
	outputs.resize(NUM_OUTPUTS);
}

void Links::step() {
	float in1 = getf(inputs[A1_INPUT]);
	float in2 = getf(inputs[B1_INPUT]) + getf(inputs[B2_INPUT]);
	float in3 = getf(inputs[C1_INPUT]) + getf(inputs[C2_INPUT]) + getf(inputs[C3_INPUT]);

	setf(outputs[A1_OUTPUT], in1);
	setf(outputs[A2_OUTPUT], in1);
	setf(outputs[A3_OUTPUT], in1);
	setf(outputs[B1_OUTPUT], in2);
	setf(outputs[B2_OUTPUT], in2);
	setf(outputs[C1_OUTPUT], in3);

	lights[0] = in1;
	lights[1] = in2;
	lights[2] = in3;
}


LinksWidget::LinksWidget() : ModuleWidget(new Links()) {
	box.size = Vec(15*4, 380);

	{
		AudiblePanel *panel = new AudiblePanel();
		panel->imageFilename = "plugins/AudibleInstruments/res/Links.png";
		panel->box.size = box.size;
		addChild(panel);
	}

	addChild(createScrew(Vec(15, 0)));
	addChild(createScrew(Vec(15, 365)));

	addInput(createInput(Vec(6-1, 78-1), module, Links::A1_INPUT));
	addOutput(createOutput(Vec(34-1, 78-1), module, Links::A1_OUTPUT));
	addOutput(createOutput(Vec(6-1, 116-1), module, Links::A2_OUTPUT));
	addOutput(createOutput(Vec(34-1, 116-1), module, Links::A3_OUTPUT));

	addInput(createInput(Vec(6-1, 180-1), module, Links::B1_INPUT));
	addInput(createInput(Vec(34-1, 180-1), module, Links::B2_INPUT));
	addOutput(createOutput(Vec(6-1, 217-1), module, Links::B1_OUTPUT));
	addOutput(createOutput(Vec(34-1, 217-1), module, Links::B2_OUTPUT));

	addInput(createInput(Vec(6-1, 281-1), module, Links::C1_INPUT));
	addInput(createInput(Vec(34-1, 281-1), module, Links::C2_INPUT));
	addInput(createInput(Vec(6-1, 319-1), module, Links::C3_INPUT));
	addOutput(createOutput(Vec(34-1, 319-1), module, Links::C1_OUTPUT));

	Links *links = dynamic_cast<Links*>(module);
	addChild(createValueLight<SmallValueLight>(Vec(26, 61), &links->lights[0]));
	addChild(createValueLight<SmallValueLight>(Vec(26, 164), &links->lights[1]));
	addChild(createValueLight<SmallValueLight>(Vec(26, 264), &links->lights[2]));
}
