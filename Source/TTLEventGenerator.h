//This prevents include loops. We recommend changing the macro to a name suitable for your plugin
#ifndef TTLEVENTS_H_DEFINED
#define TTLEVENTS_H_DEFINED

#include <ProcessorHeaders.h>

class TTLEventGenerator : public GenericProcessor
{
public:
	/** The class constructor, used to initialize any members. */
	TTLEventGenerator();

	/** The class destructor, used to deallocate memory */
	~TTLEventGenerator();

	/** If the processor has a custom editor, this method must be defined to instantiate it. */
	AudioProcessorEditor* createEditor() override;

	/** Called every time the settings of an upstream plugin are changed.
		Allows the processor to handle variations in the channel configuration or any other parameter
		passed through signal chain. The processor can use this function to modify channel objects that
		will be passed to downstream plugins. */
	void updateSettings() override;

	/** Defines the functionality of the processor.
		The process method is called every time a new data buffer is available.
		Visualizer plugins typically use this method to send data to the canvas for display purposes */
	void process(AudioBuffer<float>& buffer) override;

	/** Called immediately prior to the start of data acquisition, once all processors in the signal chain have
        indicated they are ready to process data.
         
         Returns: true if processor is ready to acquire, false otherwise
     */
    bool startAcquisition() override;

	/** Called whenever a parameter's value is changed */
    void parameterValueChanged(Parameter* param) override;

private:
	int counter; // counts the total number of incoming samples
	bool state; // holds the channel state (on or off)

	bool shouldTriggerEvent;
	bool eventWasTriggered;
   	int triggeredEventCounter;
	   
	float eventIntervalMs;
	int outputLine;

};

#endif