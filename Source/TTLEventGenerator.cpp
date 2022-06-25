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

#include "TTLEventGenerator.h"
#include "TTLEventGeneratorEditor.h"

TTLEventGenerator::TTLEventGenerator() : GenericProcessor("TTL Event Generator")
{
   shouldTriggerEvent = false;
   eventWasTriggered = false;
   triggeredEventCounter = 0;

   eventIntervalMs = 50.0f;
   outputLine = 0;

   // Parameter for manually generating events
   addStringParameter(Parameter::GLOBAL_SCOPE, "manual_trigger", "Manually trigger TTL events", String());

   // Event frequency
   addFloatParameter(Parameter::GLOBAL_SCOPE, "frequency", "Generate events at regaular intervals", 50.0f, 5.0f, 5000.0f, 5.0f);

   StringArray outputs;
   for(int i = 1; i <= 8; i++)
      outputs.add(String(i));

   // Event output line
   addCategoricalParameter(Parameter::GLOBAL_SCOPE, "out", "Event output line", outputs, 0);
}


TTLEventGenerator::~TTLEventGenerator()
{

}


AudioProcessorEditor* TTLEventGenerator::createEditor()
{
	editor = std::make_unique<TTLEventGeneratorEditor>(this);
   return editor.get();
}


void TTLEventGenerator::updateSettings()
{
   // create and add a default TTL channel to the first data stream
   addTTLChannel("TTL Event Generator Output");
}


bool TTLEventGenerator::startAcquisition()
{
   counter = 0;
   state = false;

   return true;
}


void TTLEventGenerator::parameterValueChanged(Parameter* param)
{
   if (param->getName().equalsIgnoreCase("manual_trigger"))
   {   
      shouldTriggerEvent = true;
   }
   else if(param->getName().equalsIgnoreCase("frequency"))
   {
      eventIntervalMs = (float)param->getValue();
   }
   else if(param->getName().equalsIgnoreCase("out"))
   {
      outputLine = (int)param->getValue() - 1;
   }
}


void TTLEventGenerator::process(AudioSampleBuffer& buffer)
{
	// loop through the streams
   for (auto stream : getDataStreams())
   {
      // Only generate on/off event for the first data stream
      if(stream == getDataStreams()[0])
      {
         int totalSamples = getNumSamplesInBlock(stream->getStreamId());

         int eventIntervalInSamples = (int) stream->getSampleRate() * eventIntervalMs / 2 / 1000;

         if (shouldTriggerEvent)
         {

            // add an ON event at the first sample.
            setTTLState(0, outputLine, true);

            shouldTriggerEvent = false;
            eventWasTriggered = true;
            triggeredEventCounter = 0;
         }

         for (int i = 0; i < totalSamples; i++)
         {
            counter++;

            if (eventWasTriggered)
               triggeredEventCounter++;

            if (triggeredEventCounter == eventIntervalInSamples)
            {
               setTTLState(i, outputLine, false);

               eventWasTriggered = false;
               triggeredEventCounter = 0;
            }
            
            if (counter == eventIntervalInSamples)
            {

               state = !state;
               setTTLState(i, outputLine, state);
               counter = 0;

            }

            if (counter > eventIntervalInSamples)
               counter = 0;
         }
      }
   }
	 
}

