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

//Change all names for the relevant ones, including "Processor Name"
TTLEventGenerator::TTLEventGenerator() : GenericProcessor("TTL Event Generator")
{
	setProcessorType (PROCESSOR_TYPE_FILTER);

   shouldTriggerEvent = false;
   eventWasTriggered = false;
   triggeredEventCounter = 0;

   eventIntervalMs = 50.0f;
   outputBit = 0;
}


TTLEventGenerator::~TTLEventGenerator()
{

}


AudioProcessorEditor* TTLEventGenerator::createEditor()
{
	editor = new TTLEventGeneratorEditor(this, true);
    return editor;
}


void TTLEventGenerator::createEventChannels()
{

   sampleRate = (int) getSampleRate(0);

   const DataChannel* inputChannel = getDataChannel(0);

   if (!inputChannel) // no input channels to this plugin
   {
         eventChannel = new EventChannel(EventChannel::TTL, // channel type
                                          8, // number of sub-channels
                                          1, // data packet size
                                          sampleRate, // sampleRate
                                          this); // source processor
   } else {
      eventChannel = new EventChannel(EventChannel::TTL, // channel type
                                          8, // number of sub-channels
                                          1, // data packet size
                                          inputChannel, // pointer to input channel
                                          this); // source processor
   }

   eventChannelArray.add(eventChannel); // eventChannelArray is an OwnedArray, which will
                                        // delete the eventChannel object each time
                                        // update() is called

}


bool TTLEventGenerator::enable()
{
   counter = 0;
   state = false;

   return isEnabled;
}


void TTLEventGenerator::setParameter(int ID, float value)
{
   if (ID == 0)
   {
      shouldTriggerEvent = true;
   } else if (ID == 1)
   {
      eventIntervalMs = value;
   } else if (ID == 2)
   {
      outputBit = int(value);
   }
}


void TTLEventGenerator::process(AudioSampleBuffer& buffer)
{
	int totalSamples = getNumSamples(0);

   int eventIntervalInSamples = (int) sampleRate * eventIntervalMs / 2 / 1000;

   if (shouldTriggerEvent)
   {

      // add an event at the first sample.
      uint8 ttlData = true << outputBit;

      TTLEventPtr event = TTLEvent::createTTLEvent(eventChannel,
                                                   getTimestamp(0),
                                                   &ttlData,
                                                   sizeof(uint8),
                                                   outputBit);

      addEvent(eventChannel, event, 0);

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
         uint8 ttlData = 0;

         TTLEventPtr event = TTLEvent::createTTLEvent(eventChannel,
                                                      getTimestamp(0) + i,
                                                      &ttlData,
                                                      sizeof(uint8),
                                                      outputBit);

         addEvent(eventChannel, event, i);

         eventWasTriggered = false;
         triggeredEventCounter = 0;
      }
      
      if (counter == eventIntervalInSamples)
      {

         state = !state;

         uint8 ttlData = state << outputBit;

         TTLEventPtr event = TTLEvent::createTTLEvent(eventChannel,
                                                      getTimestamp(0) + i,
                                                      &ttlData,
                                                      sizeof(uint8),
                                                      outputBit);

         addEvent(eventChannel, event, i);

         counter = 0;

      }

      if (counter > eventIntervalInSamples)
         counter = 0;
   }
	 
}

