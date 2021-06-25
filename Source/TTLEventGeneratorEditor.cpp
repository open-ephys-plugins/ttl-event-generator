/*
------------------------------------------------------------------
This file is part of the Open Ephys GUI
Copyright (C) 2021 Open Ephys
------------------------------------------------------------------
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include "TTLEventGenerator.h"
#include "TTLEventGeneratorEditor.h"

TTLEventGeneratorEditor::TTLEventGeneratorEditor(TTLEventGenerator* parentNode, bool useDefaultParameterEditors = true)
	: GenericEditor(parentNode, useDefaultParameterEditors)

{

	processor = parentNode;

	setDesiredWidth(250);

	manualTrigger = new UtilityButton("Trigger", Font("Default", 20, Font::plain));
    manualTrigger->addListener(this);
	manualTrigger->setBounds(130,35,75,25);
    addAndMakeVisible(manualTrigger);

	eventFrequency = new Slider();
	eventFrequency->setRange(50, 5000, 50);
	eventFrequency->setValue(50);
	eventFrequency->setTextValueSuffix (" ms");
	eventFrequency->addListener (this);
	eventFrequency->setChangeNotificationOnlyOnRelease(true);
	eventFrequency->setBounds(25, 95, 200, 25);
	addAndMakeVisible (eventFrequency);

	frequencyLabel = new Label("FrequencyLabel", "Frequency");
	frequencyLabel->attachToComponent(eventFrequency, false);
	addAndMakeVisible(frequencyLabel);

	outputBitSelector = new ComboBox();

	for (int bit = 1; bit <= 8; bit++)
		outputBitSelector->addItem(String(bit), bit);

	outputBitSelector->setSelectedId(1);
	outputBitSelector->setBounds(50,35,50,25);
	outputBitSelector->setTooltip("Output event channel");
	outputBitSelector->addListener(this);
	addAndMakeVisible(outputBitSelector);

	outputLabel = new Label("Bit Label", "OUT");
	outputLabel->attachToComponent(outputBitSelector, true);
	addAndMakeVisible(outputLabel);

}

TTLEventGeneratorEditor::~TTLEventGeneratorEditor()
{

}

void TTLEventGeneratorEditor::buttonEvent(Button* button)
{
	if(button == manualTrigger)
	{
		processor->setParameter(0, 0.0f); // the second input (value) is arbitrary in this case
	}
}

void TTLEventGeneratorEditor::sliderEvent(Slider* slider)
{
	if(slider == eventFrequency)
	{
		processor->setParameter(1, slider->getValue());
	}
}

void TTLEventGeneratorEditor::comboBoxChanged(ComboBox* comboBox)
{
	if(comboBox == outputBitSelector)
	{
		processor->setParameter(2, comboBox->getSelectedId() - 1); // subtract 1 to convert to zero-based indexing
	}
}

void TTLEventGeneratorEditor::updateSettings()
{


}
