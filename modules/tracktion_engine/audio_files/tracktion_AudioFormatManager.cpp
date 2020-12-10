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

AudioFileFormatManager::AudioFileFormatManager()
{
    CRASH_TRACER

    // NB default must be first!

    wavFormat = std::make_unique<juce::WavAudioFormat>();
    readFormats.add (wavFormat.get());
    writeFormats.add (wavFormat.get());

    aiffFormat = std::make_unique<juce::AiffAudioFormat>();
    readFormats.add (aiffFormat.get());
    writeFormats.add (aiffFormat.get());

    floatFormat = std::make_unique<FloatAudioFormat>();

   #if TRACKTION_USE_FLOAT_FORMAT
    readFormats.add (floatFormat.get());
   #endif

   #if JUCE_USE_OGGVORBIS
    oggFormat = std::make_unique<juce::OggVorbisAudioFormat>();
    readFormats.add (oggFormat.get());
    writeFormats.add (oggFormat.get());
   #endif

   #if JUCE_USE_FLAC
    flacFormat = std::make_unique<juce::FlacAudioFormat>();
    readFormats.add (flacFormat.get());
    writeFormats.add (flacFormat.get());
   #endif

   #if TRACKTION_ENABLE_REX
    rexFormat = std::make_unique<RexAudioFormat>();
    readFormats.add (rexFormat.get());
   #endif

   #if JUCE_MAC || JUCE_IOS
    nativeAudioFormat = std::make_unique<juce::CoreAudioFormat>();
    readFormats.add (nativeAudioFormat.get());
   #endif

   #if JUCE_USE_WINDOWS_MEDIA_FORMAT
    nativeAudioFormat = std::make_unique<juce::WindowsMediaAudioFormat>();
    readFormats.add (nativeAudioFormat.get());
   #endif

   #if JUCE_USE_MP3AUDIOFORMAT
    mp3ReadFormat = std::make_unique<juce::MP3AudioFormat>();
    readFormats.add (mp3ReadFormat.get());
   #endif

    // NB when adding new formats -
    //  this bit assumes all writable formats are also readable
    allFormats = readFormats;

    readFormatManager.registerFormat (new juce::WavAudioFormat(), true);
    readFormatManager.registerFormat (new juce::AiffAudioFormat(), false);

   #if TRACKTION_USE_FLOAT_FORMAT
    readFormatManager.registerFormat (new FloatAudioFormat(), false);
   #endif

   #if JUCE_USE_OGGVORBIS
    readFormatManager.registerFormat (new juce::OggVorbisAudioFormat(), false);
   #endif

   #if JUCE_USE_FLAC
    readFormatManager.registerFormat (new juce::FlacAudioFormat(), false);
   #endif

   #if TRACKTION_ENABLE_REX
    readFormatManager.registerFormat (new RexAudioFormat(), false);
   #endif

   #if JUCE_MAC || JUCE_IOS
    readFormatManager.registerFormat (new juce::CoreAudioFormat(), false);
   #endif

   #if JUCE_USE_WINDOWS_MEDIA_FORMAT
    readFormatManager.registerFormat (new juce::WindowsMediaAudioFormat(), false);
   #endif

   #if JUCE_USE_MP3AUDIOFORMAT
    readFormatManager.registerFormat (new juce::MP3AudioFormat(), false);
   #endif

    writeFormatManager.registerFormat (new juce::WavAudioFormat(), true);
    writeFormatManager.registerFormat (new juce::AiffAudioFormat(), false);

   #if TRACKTION_USE_FLOAT_FORMAT
    writeFormatManager.registerFormat (new FloatAudioFormat(), false);
   #endif

   #if JUCE_USE_OGGVORBIS
    writeFormatManager.registerFormat (new juce::OggVorbisAudioFormat(), false);
   #endif

   #if JUCE_USE_FLAC
    writeFormatManager.registerFormat (new juce::FlacAudioFormat(), false);
   #endif

    memoryMappedFormatManager.registerFormat (new juce::WavAudioFormat(), true);
    memoryMappedFormatManager.registerFormat (new juce::AiffAudioFormat(), false);

   #if TRACKTION_USE_FLOAT_FORMAT
    memoryMappedFormatManager.registerFormat (new FloatAudioFormat(), false);
   #endif
}

AudioFileFormatManager::~AudioFileFormatManager()
{
    CRASH_TRACER
}

#if JUCE_USE_LAME_AUDIO_FORMAT
void AudioFileFormatManager::addLameFormat (std::unique_ptr<juce::AudioFormat> lameForArray,
                                            std::unique_ptr<juce::AudioFormat> lameForAccess)
{
    if (lameFormat != nullptr || lameForArray == nullptr || lameForAccess == nullptr)
        return;

    lameFormat = std::move (lameForAccess);
    writeFormats.add (lameForArray.get());
    writeFormatManager.registerFormat (lameForArray.release(), false);
}
#endif

void AudioFileFormatManager::addFormat (std::function<juce::AudioFormat*()> formatCreator, bool isWritable, bool isMemoryMappable)
{
    jassert (formatCreator);

    auto* af = additionalFormats.add (formatCreator());
    readFormats.add (af);
    allFormats.add (af);

    readFormatManager.registerFormat (formatCreator(), false);

    if (isWritable)
        writeFormatManager.registerFormat (formatCreator(), false);

    if (isMemoryMappable)
        memoryMappedFormatManager.registerFormat (formatCreator(), false);
}

juce::AudioFormat* AudioFileFormatManager::getFormatFromFileName (const juce::File& f) const
{
    for (auto format : allFormats)
        if (format->canHandleFile (f))
            return format;

    return {};
}

bool AudioFileFormatManager::canOpen (const juce::File& f) const
{
    return getFormatFromFileName (f) != nullptr;
}

juce::String AudioFileFormatManager::getValidFileExtensions() const
{
    return "wav;aiff;aif;ogg;mp3;mid;midi;flac;au;voc;caf;w64;rx2;rcy;rex;m4a;wfaf";
}

juce::AudioFormat* AudioFileFormatManager::getNamedFormat (const juce::String& formatName) const
{
    for (auto format : allFormats)
        if (format->getFormatName() == formatName)
            return format;

    return getDefaultFormat();
}

}
