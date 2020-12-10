/*
    ,--.                     ,--.     ,--.  ,--.
  ,-'  '-.,--.--.,--,--.,---.|  |,-.,-'  '-.`--' ,---. ,--,--,      Copyright 2018
  '-.  .-'|  .--' ,-.  | .--'|     /'-.  .-',--.| .-. ||      \   Tracktion Software
    |  |  |  |  \ '-'  \ `--.|  \  \  |  |  |  |' '-' '|  ||  |       Corporation
    `---' `--'   `--`--'`---'`--'`--' `---' `--' `---' `--''--'    www.tracktion.com

    Tracktion Engine uses a GPL/commercial licence - see LICENCE.md for details.
*/

namespace tracktion_engine
{

/** Keeps a list of available wave formats and can create a format object
    for a file.
*/
class AudioFileFormatManager
{
public:
    AudioFileFormatManager();
    ~AudioFileFormatManager();

    //==============================================================================
    const juce::Array<juce::AudioFormat*>& getWriteFormats() const     { return writeFormats; }

   #if JUCE_USE_LAME_AUDIO_FORMAT
    void addLameFormat (std::unique_ptr<juce::AudioFormat> lameForArray,
                        std::unique_ptr<juce::AudioFormat> lameForAccess);
   #endif

    void addFormat (std::function<juce::AudioFormat*()> formatCreator, bool isWritable, bool isMemoryMappable);

    //==============================================================================
    juce::AudioFormat* getFormatFromFileName (const juce::File&) const;
    juce::AudioFormat* getNamedFormat (const juce::String& formatName) const;

    bool canOpen (const juce::File&) const;
    juce::String getValidFileExtensions() const;

    juce::AudioFormat* getDefaultFormat() const     { return wavFormat.get(); }
    juce::AudioFormat* getWavFormat() const         { return wavFormat.get(); }
    juce::AudioFormat* getAiffFormat() const        { return aiffFormat.get(); }

    juce::AudioFormat* getFrozenFileFormat() const  { return floatFormat.get(); }

   #if JUCE_USE_OGGVORBIS
    juce::AudioFormat* getOggFormat() const         { return oggFormat.get(); }
   #endif

   #if JUCE_USE_FLAC
    juce::AudioFormat* getFlacFormat() const        { return flacFormat.get(); }
   #endif

    juce::AudioFormat* getNativeAudioFormat() const { return nativeAudioFormat.get(); }

   #if JUCE_USE_LAME_AUDIO_FORMAT
    juce::AudioFormat* getLameFormat() const        { return lameFormat.get(); }
   #endif

   #if TRACKTION_ENABLE_REX
    juce::AudioFormat* getRexFormat() const         { return rexFormat.get(); }
   #endif

    juce::AudioFormatManager readFormatManager, writeFormatManager, memoryMappedFormatManager;

private:
    juce::Array<juce::AudioFormat*> allFormats, readFormats, writeFormats;
    juce::OwnedArray<juce::AudioFormat> additionalFormats;

    std::unique_ptr<juce::AudioFormat> wavFormat, aiffFormat, floatFormat, nativeAudioFormat,
                                       mp3ReadFormat, oggFormat, flacFormat, lameFormat;

   #if TRACKTION_ENABLE_REX
    std::unique_ptr<juce::AudioFormat> rexFormat;
   #endif
};

} // namespace tracktion_engine
