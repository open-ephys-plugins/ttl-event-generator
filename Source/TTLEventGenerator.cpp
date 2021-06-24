#include "TTLEventGenerator.h"

//Change all names for the relevant ones, including "Processor Name"
TTLEventGenerator::TTLEventGenerator() : GenericProcessor("TTL Event Generator")
{

}

TTLEventGenerator::~TTLEventGenerator()
{

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

void TTLEventGenerator::process(AudioSampleBuffer& buffer)
{
	int totalSamples = getNumSamples(0);

   for (int i = 0; i < totalSamples; i++)
   {
      counter++;

      if (counter == sampleRate)
      {

         state = !state;

         uint8 ttlData = state << 0;

         TTLEventPtr event = TTLEvent::createTTLEvent(eventChannel,
                                                      getTimestamp(0) + i,
                                                      &ttlData,
                                                      sizeof(uint8),
                                                      0);

         addEvent(eventChannel, event, i);

         counter = 0;

      }
   }
	 
}

