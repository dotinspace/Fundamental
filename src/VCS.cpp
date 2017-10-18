// Some simple voltage controlled switches

#include "MrLumps.hpp"
#include "dsp/digital.hpp"

#define BG_IMAGE_FILE_1x8  assetPlugin(plugin, "res/VCS1x8.png")
#define BG_IMAGE_FILE_2x4  assetPlugin(plugin, "res/VCS2x4.png")



struct VCS1x8 : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		TRIGGER_INPUT,
		SIGNAL_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		SWITCHED_OUTPUT,
		NUM_OUTPUTS = SWITCHED_OUTPUT + 8
	};

	SchmittTrigger inputTrigger;
	int currentSwitch;

	// Lights
	float outputLights[8];

	VCS1x8() {
			params.resize(NUM_PARAMS);
			inputs.resize(NUM_INPUTS);
			outputs.resize(NUM_OUTPUTS);
			currentSwitch = 0;
			outputLights[currentSwitch] = 10.0f;
	}

	// Called via menu
	void initialize() {
		for (int c = 0; c < 8; c++) {
			outputLights[c] = 0.0f;
			outputs[c].value = 0.0f;
		}
		currentSwitch = 0;
		outputLights[currentSwitch] = 10.0f;
	}

	//Todo
	void randomize() {
	}

	void step();
};

//Lets try simplest 1st
//Just start at 0 and walk around the outputs on triggers
void VCS1x8::step() {

	if (inputs[TRIGGER_INPUT].active) {
		// External clock
		if (inputTrigger.process(inputs[TRIGGER_INPUT].value)) {
			//Switch ports to the next active port

			//Search forward from the current switch
			for (int i = currentSwitch+1; i < NUM_OUTPUTS; i++) {
				if (outputs[i].active) {
					outputLights[currentSwitch] = 0.0f;
					outputs[currentSwitch].value = 0.0f;
					outputLights[i] = 10.0f;
					currentSwitch = i;
					goto OUTPUT;
				}
			}
			//Else wrap around
			for (int i = 0; i < currentSwitch; i++) {
				if (outputs[i].active) {
					outputLights[currentSwitch] = 0.0f;
					outputs[currentSwitch].value = 0.0f;
					outputLights[i] = 10.0f;
					currentSwitch = i;
					goto OUTPUT;
				}
			}
			//Okay we have nothing to switch to
		}
	}


OUTPUT:

	//We have signal so send output
	if (inputs[SIGNAL_INPUT].active) {
		if (outputs[currentSwitch].active) {
			outputs[currentSwitch].value = inputs[SIGNAL_INPUT].value;
		}
	}

}

	VCS1x8Widget::VCS1x8Widget() {
		VCS1x8 *module = new VCS1x8();

		setModule(module);
		box.size = Vec(15 * 4, 380);

		{
			Panel *panel = new LightPanel();
			panel->backgroundImage = Image::load(BG_IMAGE_FILE_1x8);
			panel->box.size = box.size;
			addChild(panel);
		}

		addChild(createScrew<ScrewSilver>(Vec(15, 0)));
		addChild(createScrew<ScrewSilver>(Vec(15, 365)));

		const float bankX[8] = { 4, 31, 4, 31, 4, 31, 4, 31 };
		const float bankY[8] = { 112, 112, 179, 179, 246, 246, 313, 313 };

		//Trigger input
		addInput(createInput<PJ3410Port>(Vec(29, 23), module, VCS1x8::TRIGGER_INPUT));

		//Signal input
		addInput(createInput<PJ3410Port>(Vec(29, 57), module, VCS1x8::SIGNAL_INPUT));

		//Switched ouputs + lights
		for (int outputs = 0; outputs < 8; outputs++) {
			addChild(createValueLight<SmallLight<RedValueLight>>(Vec(bankX[outputs] + 9, bankY[outputs] - 12), &module->outputLights[outputs]));
			addOutput(createOutput<PJ301MPort>(Vec(bankX[outputs], bankY[outputs]), module, VCS1x8::SWITCHED_OUTPUT + outputs));
		}


}







struct VCS2x4 : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		TRIGGER_INPUT,
		SIGNAL_INPUT_L,
		SIGNAL_INPUT_R,
		NUM_INPUTS
	};
	enum OutputIds {
		SWITCHED_OUTPUT_L,
		SWITCHED_OUTPUT_L2,
		SWITCHED_OUTPUT_L3,
		SWITCHED_OUTPUT_L4,
		SWITCHED_OUTPUT_R,
		SWITCHED_OUTPUT_R2,
		SWITCHED_OUTPUT_R3,
		SWITCHED_OUTPUT_R4,
		NUM_OUTPUTS
	};

	SchmittTrigger inputTrigger;
	int currentSwitchL;
	int currentSwitchR;

	// Lights
	float outputLights[8];


	VCS2x4() {
		params.resize(NUM_PARAMS);
		inputs.resize(NUM_INPUTS);
		outputs.resize(NUM_OUTPUTS);
		currentSwitchL = 0;
		currentSwitchR = SWITCHED_OUTPUT_R;
		outputLights[currentSwitchL] = 10.0f;
		outputLights[currentSwitchR] = 10.0f;
	}

	// Called via menu
	void initialize() {
		for (int c = 0; c < 4; c++) {
			outputLights[c] = 0.0f;   // Left
			outputLights[c+4] = 0.0f; // Right
		}
		outputs[currentSwitchL].value = 0.0f;
		outputs[currentSwitchR].value = 0.0f;
		currentSwitchL = 0;
		currentSwitchR = SWITCHED_OUTPUT_R;
		outputLights[currentSwitchL] = 10.0f;
		outputLights[currentSwitchR] = 10.0f;
	}

	//Todo
	void randomize() {
	}

	void step();
};

//Lets try simplest 1st
//Just start at 0 and walk around the outputs on triggers
void VCS2x4::step() {

	if (inputs[TRIGGER_INPUT].active) {
		// External clock
		if (inputTrigger.process(inputs[TRIGGER_INPUT].value)) {
			//Switch ports to the next active port


			// L Bank
			{
				//Search forward from the current switch
				for (int i = currentSwitchL + 1; i < NUM_OUTPUTS/2; i++) {
					if (outputs[i].active) {
						outputLights[currentSwitchL] = 0.0f;
						outputs[currentSwitchL].value = 0.0f;
						outputLights[i] = 10.0f;
						currentSwitchL = i;
						goto NEXTSIDE;
					}
				}
				//Else wrap around
				for (int i = SWITCHED_OUTPUT_L; i < currentSwitchL; i++) {
					if (outputs[i].active) {
						outputLights[currentSwitchL] = 0.0f;
						outputs[currentSwitchL].value = 0.0f;
						outputLights[i] = 10.0f;
						currentSwitchL = i;
						goto NEXTSIDE;
					}
				}
				//Okay we have nothing to switch to, next bank
			}
		NEXTSIDE:
			// R Bank
			{
				//Search forward from the current switch
				for (int i = currentSwitchR + 1; i < NUM_OUTPUTS; i++) {
					if (outputs[i].active) {
						outputLights[currentSwitchR] = 0.0f;
						outputs[currentSwitchR].value = 0.0f;
						outputLights[i] = 10.0f;
						currentSwitchR = i;
						goto OUTPUT;
					}
				}
				//Else wrap around
				for (int i = SWITCHED_OUTPUT_R; i < currentSwitchR; i++) {
					if (outputs[i].active) {
						outputLights[currentSwitchR] = 0.0f;
						outputs[currentSwitchR].value = 0.0f;
						outputLights[i] = 10.0f;
						currentSwitchR = i;
						goto OUTPUT;
					}
				}
				//Okay we have nothing to switch to
			}
		}
	}


OUTPUT:

	//We have signal so send output
	if (inputs[SIGNAL_INPUT_L].active) {
		if (outputs[currentSwitchL].active) {
			outputs[currentSwitchL].value = inputs[SIGNAL_INPUT_L].value;
		}
	}

	if (inputs[SIGNAL_INPUT_R].active) {
		if (outputs[currentSwitchR].active) {
			outputs[currentSwitchR].value = inputs[SIGNAL_INPUT_R].value;
		}
	}

}


VCS2x4Widget::VCS2x4Widget() {
	VCS2x4 *module = new VCS2x4();

	setModule(module);
	box.size = Vec(15 * 4, 380);

	{
		Panel *panel = new LightPanel();
		panel->backgroundImage = Image::load(BG_IMAGE_FILE_2x4);
		panel->box.size = box.size;
		addChild(panel);
	}

	addChild(createScrew<ScrewSilver>(Vec(15, 0)));
	addChild(createScrew<ScrewSilver>(Vec(15, 365)));

	const float bankX[2] = { 4, 31 };
	const float bankY[4] = { 112, 179, 246, 313 };

	//Trigger input
	addInput(createInput<PJ3410Port>(Vec(29, 23), module, VCS2x4::TRIGGER_INPUT));

	//Signal inputs
	addInput(createInput<PJ3410Port>(Vec(0, 52), module, VCS2x4::SIGNAL_INPUT_L));
	addInput(createInput<PJ3410Port>(Vec(29, 52), module, VCS2x4::SIGNAL_INPUT_R));

	//Switched ouputs + lights
	for (int outputs = 0; outputs < 4; outputs++) {
		addChild(createValueLight<SmallLight<RedValueLight>>(Vec(bankX[0] + 9, bankY[outputs] - 12), &module->outputLights[outputs]));
		addOutput(createOutput<PJ301MPort>(Vec(bankX[0], bankY[outputs]), module, VCS2x4::SWITCHED_OUTPUT_L + outputs));

		addChild(createValueLight<SmallLight<RedValueLight>>(Vec(bankX[1] + 9, bankY[outputs] - 12), &module->outputLights[outputs+4]));
		addOutput(createOutput<PJ301MPort>(Vec(bankX[1], bankY[outputs]), module, VCS2x4::SWITCHED_OUTPUT_R + outputs));
	}

}



