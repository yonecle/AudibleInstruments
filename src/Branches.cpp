#include "AudibleInstruments.hpp"


struct Branches : Module {
	enum ParamIds {
		THRESHOLD1_PARAM,
		THRESHOLD2_PARAM,
		MODE1_PARAM,
		MODE2_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		IN1_INPUT,
		P1_INPUT,
		IN2_INPUT,
		P2_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1A_OUTPUT,
		OUT1B_OUTPUT,
		OUT2A_OUTPUT,
		OUT2B_OUTPUT,
		NUM_OUTPUTS
	};

	bool lastGate[2] = {};
	bool outcome[2] = {};
	float light[2] = {};

	Branches();
	void step();
	float getOutput(int outputId);
};


Branches::Branches() {
	params.resize(NUM_PARAMS);
	inputs.resize(NUM_INPUTS);
	outputs.resize(NUM_OUTPUTS);
}

static void computeChannel(const float *in, const float *p, float threshold, float mode, bool *lastGate, bool *outcome, float *out1, float *out2, float *light) {
	float out = getf(in);
	bool gate = (out >= 1.0);
	if (gate && !*lastGate) {
		// trigger
		std::uniform_real_distribution<float> dist(0.0, 1.0);
		bool toss = (dist(rng) < threshold + getf(p));
		if (mode < 0.5) {
			// direct mode
			*outcome = toss;
		}
		else {
			// toggle mode
			*outcome = *outcome != toss;
		}
	}
	*lastGate = gate;
	*light = *outcome ? out : -out;

	if (out1) {
		*out1 = *outcome ? 0.0 : out;
	}
	if (out2) {
		*out2 = *outcome ? out : 0.0;
	}
}

void Branches::step() {
	computeChannel(inputs[IN1_INPUT], inputs[P1_INPUT], params[THRESHOLD1_PARAM], params[MODE1_PARAM], &lastGate[0], &outcome[0], outputs[OUT1A_OUTPUT], outputs[OUT1B_OUTPUT], &light[0]);
	computeChannel(inputs[IN2_INPUT], inputs[P2_INPUT], params[THRESHOLD2_PARAM], params[MODE2_PARAM], &lastGate[1], &outcome[1], outputs[OUT2A_OUTPUT], outputs[OUT2B_OUTPUT], &light[1]);
}


BranchesWidget::BranchesWidget() : ModuleWidget(new Branches()) {
	box.size = Vec(15*6, 380);

	{
		AudiblePanel *panel = new AudiblePanel();
		panel->imageFilename = "plugins/AudibleInstruments/res/Branches.png";
		panel->box.size = box.size;
		addChild(panel);
	}

	addChild(createScrew(Vec(15, 0)));
	addChild(createScrew(Vec(15, 365)));

	addParam(createParam<SmallRedKnob>(Vec(44-20, 84-20), module, Branches::THRESHOLD1_PARAM, 0.0, 1.0, 0.5));
	addParam(createParam<MediumToggleSwitch>(Vec(69, 58), module, Branches::MODE1_PARAM, 0.0, 1.0, 0.0));
	addInput(createInput(Vec(11, 125-1), module, Branches::IN1_INPUT));
	addInput(createInput(Vec(58, 125-1), module, Branches::P1_INPUT));
	addOutput(createOutput(Vec(11, 163-1), module, Branches::OUT1A_OUTPUT));
	addOutput(createOutput(Vec(58, 163-1), module, Branches::OUT1B_OUTPUT));

	addParam(createParam<SmallGreenKnob>(Vec(44-20, 240-20), module, Branches::THRESHOLD2_PARAM, 0.0, 1.0, 0.5));
	addParam(createParam<MediumToggleSwitch>(Vec(69, 214), module, Branches::MODE2_PARAM, 0.0, 1.0, 0.0));
	addInput(createInput(Vec(11, 281-1), module, Branches::IN2_INPUT));
	addInput(createInput(Vec(58, 281-1), module, Branches::P2_INPUT));
	addOutput(createOutput(Vec(11, 319-1), module, Branches::OUT2A_OUTPUT));
	addOutput(createOutput(Vec(58, 319-1), module, Branches::OUT2B_OUTPUT));

	Branches *branches = dynamic_cast<Branches*>(module);
	addChild(createValueLight<SmallValueLight>(Vec(42, 170), &branches->light[0]));
	addChild(createValueLight<SmallValueLight>(Vec(42, 326), &branches->light[1]));
}
