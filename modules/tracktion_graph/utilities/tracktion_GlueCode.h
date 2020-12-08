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
/** Converts a choc::buffer::BufferView to a juce::dsp::AudioBlock<float>. */
template <typename SampleType, template<typename> typename LayoutType>
juce::dsp::AudioBlock<float> toAudioBlock (const choc::buffer::BufferView<SampleType, LayoutType>& view)
{
    return juce::dsp::AudioBlock<float> (view.data.channels,
                                         view.size.numChannels,
                                         view.size.numFrames);
}

/** Converts a choc::buffer to a juce::dsp::AudioBlock<float>. */
template<typename ChocBufferType>
juce::dsp::AudioBlock<float> toAudioBlock (const ChocBufferType& buffer)
{
    return toAudioBlock (buffer.getView());
}

/** Creates a juce::AudioBuffer from a juce::AudioBlock. */
inline juce::AudioBuffer<float> createAudioBuffer (const juce::dsp::AudioBlock<float>& block)
{
    constexpr int maxNumChannels = 128;
    const int numChannels = std::min (maxNumChannels, (int) block.getNumChannels());
    float* chans[maxNumChannels] = {};

    for (int i = 0; i < numChannels; ++i)
        chans[i] = block.getChannelPointer ((size_t) i);

    return juce::AudioBuffer<float> (chans, numChannels, (int) block.getNumSamples());
}

/** Creates a juce::AudioBuffer from a choc::buffer::BufferView. */
template <typename SampleType, template<typename> typename LayoutType>
inline juce::AudioBuffer<float> createAudioBuffer (const choc::buffer::BufferView<SampleType, LayoutType>& view)
{
    return createAudioBuffer (toAudioBlock (view));
}

/** Creates a juce::AudioBuffer from a choc::buffer. */
template<typename ChocBufferType>
inline juce::AudioBuffer<float> createAudioBuffer (const ChocBufferType& buffer)
{
    return createAudioBuffer (toAudioBlock (buffer));
}

/** Converts a juce::dsp::AudioBlock<float> to a choc::buffer::BufferView. */
inline choc::buffer::BufferView<float, choc::buffer::SeparateChannelLayout> toBufferView (const juce::dsp::AudioBlock<float>& block)
{
    constexpr int maxNumChannels = 128;
    const int numChannels = std::min (maxNumChannels, (int) block.getNumChannels());
    float* chans[maxNumChannels] = {};

    for (int i = 0; i < numChannels; ++i)
        chans[i] = block.getChannelPointer ((size_t) i);

    return choc::buffer::createChannelArrayView (chans,
                                                 (choc::buffer::ChannelCount) block.getNumChannels(),
                                                 (choc::buffer::FrameCount) block.getNumSamples());
}



} // namespace tracktion_graph
