/*
    ,--.                     ,--.     ,--.  ,--.
  ,-'  '-.,--.--.,--,--.,---.|  |,-.,-'  '-.`--' ,---. ,--,--,      Copyright 2018
  '-.  .-'|  .--' ,-.  | .--'|     /'-.  .-',--.| .-. ||      \   Tracktion Software
    |  |  |  |  \ '-'  \ `--.|  \  \  |  |  |  |' '-' '|  ||  |       Corporation
    `---' `--'   `--`--'`---'`--'`--' `---' `--' `---' `--''--'    www.tracktion.com

    Tracktion Engine uses a GPL/commercial licence - see LICENCE.md for details.
*/

namespace tracktion_graph
{

//==============================================================================
/** Converts a choc::buffer::BufferView to a juce::dsp::AudioBlock<float> */
template <typename SampleType, template<typename> typename LayoutType>
juce::dsp::AudioBlock<float> toAudioBlock (const choc::buffer::BufferView<SampleType, LayoutType>& view)
{
    return juce::dsp::AudioBlock<float> (view.data.channels,
                                         view.size.numChannels,
                                         view.size.numFrames);
}

/** Converts a choc::buffer to a juce::dsp::AudioBlock<float> */
template<typename ChocBufferType>
juce::dsp::AudioBlock<float> toAudioBlock (const ChocBufferType& buffer)
{
    return toAudioBlock (buffer.getView());
}

} // namespace tracktion_graph
