#include <application.hpp>

#include <unistd.h>

void
Application::InitMIDI(void)
{
    midiOut =   std::make_shared<MidiOut>();
    midiIn  =   std::make_shared<MidiIn>();

    std::cout << "Midi In: " << std::endl;
    auto    ports   =   midiIn->GetPorts();
    for (unsigned int i = 0; i < ports.size(); ++i)
        std::cout << "Port id: " << i << " name: " << ports.at(i) << std::endl;
    
    int n;
    std::cin >> n;

    midiIn->OpenPort(n);

    std::cout << "Midi Out: " << std::endl;
    ports   =   midiOut->GetPorts();
    for (unsigned int i = 0; i < ports.size(); ++i)
        std::cout << "Port id: " << i << " name: " << ports.at(i) << std::endl;

    std::cin >> n;

    std::cout << "Mode: " << std::endl <<
        "1: Color checker" << std::endl << 
        "2: Mario" << std::endl <<
        "3: Disco" << std::endl <<
        "4: Touch" << std::endl <<
        "5: Toggle" << std::endl <<
        "6: PC" << std::endl <<
        "7: Drum mode 2" << std::endl <<
        "8: Brightness" << std::endl;

    std::cin >> mode;

    midiOut->OpenPort(n);
    midiOut->SendMidiMessage(LaunchKey::DAWModeOn);

    if (mode == 1 || mode == 2 || mode == 8)
        midiOut->SendMidiMessage(LaunchKey::PadModeSession);

    if (mode == 3 || mode == 4)
        midiOut->SendMidiMessage(LaunchKey::PadModeDrum);

    if (mode == 5)
        midiOut->SendMidiMessage(LaunchKey::PadModeToggle);

    if (mode == 6)
        midiOut->SendMidiMessage(LaunchKey::PadModePC);

    if (mode == 7)
        midiOut->SendMidiMessage(LaunchKey::PadModeDrum2);

    MidiMessage message = LaunchKey::Play;
    message.channel = MidiChannel::CH2;
    message.data2 = 0x01;
    midiOut->SendMidiMessage(message);
}

void
Application::Init(void)
{
    std::cout << "Play - exit" << std::endl;
    InitMIDI();
}

void
Application::ColorPicker(std::optional<MidiMessage> message)
{
    if (!message) return;

    int tmp = numColor;
    if (CompareMidiMessage(*message, LaunchKey::ArrUp))
        numColor++;
    else if (CompareMidiMessage(*message, LaunchKey::ArrDown))
        numColor--;

    if (tmp == numColor) return;

    if (numColor < 0) numColor = 0;
    if (numColor > 127) numColor = 127;

    MidiMessage colMessage = LaunchKey::SessionPadColor;
    colMessage.data2   =   numColor;
    for (int i = 0; i < 8; ++i)
    {
        colMessage.data1   =   LaunchKey::SessionPads::SP1 + i;
        midiOut->SendMidiMessage(colMessage);
    }
    for (int i = 0; i < 8; ++i)
    {
        colMessage.data1   =   LaunchKey::SessionPads::SP9 + i;
        midiOut->SendMidiMessage(colMessage);
    }
}

void
Application::Mario(void)
{
    static int numRow = 0;
    static const std::array<std::array<int, 16>, 16> mario {{
        {0,   0,   0,   0,   0,   5,   5,   5,   5,   5,   0,   0,   0,   0,   0,   0},
        {0,   0,   0,   0,   5,   5,   5,   5,   5,   5,   5,   5,   5,   0,   0,   0},
        {0,   0,   0,   0,  11,  11,  11,   9,   9,  11,   9,   0,   0,   0,   0,   0},
        {0,   0,   0,  11,   9,  11,   9,   9,   9,  11,   9,   9,   9,   0,   0,   0},
        {0,   0,   0,  11,   9,  11,  11,   9,   9,   9,  11,   9,   9,   9,   0,   0},
        {0,   0,   0,  11,  11,   9,   9,   9,   9,  11,  11,  11,  11,   0,   0,   0},
        {0,   0,   0,   0,   0,   9,   9,   9,   9,   9,   9,   9,   0,   0,   0,   0},
        {0,   0,   0,   0,  11,  11,   5,  11,  11,  11,   0,   0,   0,   0,   0,   0},
        {0,   0,   0,  11,  11,  11,   5,  11,  11,   5,  11,  11,  11,   0,   0,   0},
        {0,   0,  11,  11,  11,  11,   5,   5,   5,   5,  11,  11,  11,  11,   0,   0},
        {0,   0,   9,   9,  11,   5,  13,   5,   5,  13,   5,  11,   9,   9,   0,   0},
        {0,   0,   9,   9,   9,   5,   5,   5,   5,   5,   5,   9,   9,   9,   0,   0},
        {0,   0,   9,   9,   5,   5,   5,   5,   5,   5,   5,   5,   9,   9,   0,   0},
        {0,   0,   0,   0,   5,   5,   5,   0,   0,   5,   5,   5,   0,   0,   0,   0},
        {0,   0,   0,  11,  11,  11,   0,   0,   0,   0,  11,  11,  11,   0,   0,   0},
        {0,   0,  11,  11,  11,  11,   0,   0,   0,   0,  11,  11,  11,  11,   0,   0}
    }};

    static bool dir = true;

    MidiMessage colMessage = LaunchKey::SessionPadColor;
    for (int i = 0; i < 8; ++i)
    {
        colMessage.data1   =   LaunchKey::SessionPads::SP1 + i;
        colMessage.data2   =   mario[numRow][i+4];
        midiOut->SendMidiMessage(colMessage);
    }

    for (int i = 0; i < 8; ++i)
    {
        colMessage.data1   =   LaunchKey::SessionPads::SP9 + i;
        colMessage.data2   =   mario[numRow+1][i+4];
        midiOut->SendMidiMessage(colMessage);
    }

    numRow += dir ? 1 : -1;
    if (numRow == 14 ||
        numRow == 0)
        dir = !dir;

    usleep(166667);
}

void
Application::Disco(void)
{
    static bool initialized = false;
    static std::array<int, 16> timers;

    if (!initialized)
    {
        for (int i = 0; i > 16; ++i)
            timers[i] = rand() % 100;
        initialized = true;
    }

    MidiMessage colMessage = LaunchKey::DrumPadColor;

    for (int i = 0; i < 4; ++i)
    {
        if (timers[i] <= 0)
        {
            colMessage.data1   =   LaunchKey::DrumPads::DP1 + i;
            colMessage.data2   =   (rand() % 14 + 1) * 4 + 1;
            midiOut->SendMidiMessage(colMessage);
            timers[i] = rand() % 5;
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        if (timers[i+4] <= 0)
        {
            colMessage.data1   =   LaunchKey::DrumPads::DP5 + i;
            colMessage.data2   =   (rand() % 14 + 1) * 4 + 1;
            midiOut->SendMidiMessage(colMessage);
            timers[i+4] = rand() % 5;
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        if (timers[i+8] <= 0)
        {
            colMessage.data1   =   LaunchKey::DrumPads::DP9 + i;
            colMessage.data2   =   (rand() % 14 + 1) * 4 + 1;
            midiOut->SendMidiMessage(colMessage);
            timers[i+8] = rand() % 5;
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        if (timers[i+12] <= 0)
        {
            colMessage.data1   =   LaunchKey::DrumPads::DP13 + i;
            colMessage.data2   =   (rand() % 14 + 1) * 4 + 1;
            midiOut->SendMidiMessage(colMessage);
            timers[i+12] = rand() % 5;
        }
    }

    for (int i = 0; i < 16; ++i)
        timers[i]--;

    usleep(100000);
}

void
Application::Touch(std::optional<MidiMessage> message)
{
    if (!message) return;

    static int colorMode = 0;

    if (CompareMidiMessage(*message, LaunchKey::ArrUp))
        colorMode++;
    else if (CompareMidiMessage(*message, LaunchKey::ArrDown))
        colorMode--;

    if (colorMode < 0) colorMode = 0;
    if (colorMode > 2) colorMode = 2;

    if (message->channel  ==  MidiChannel::CH10 &&
        message->status   ==  MidiStatus::NON &&
        message->data2.value() != 0)
    {
        MidiMessage colMessage;

        if (colorMode == 0)
            colMessage = LaunchKey::DrumPadColor;
        else if (colorMode == 1)
            colMessage = LaunchKey::DrumPadColorFlash;
        else if (colorMode == 2)
            colMessage = LaunchKey::DrumPadColorPulse;

        colMessage.data1    =   message->data1.value();
        colMessage.data2    =   rand() % 128;
        midiOut->SendMidiMessage(colMessage);
    }
}

void
Application::Brightness(void)
{
    static bool init = false;

    if (init) return;
    MidiMessage message = LaunchKey::PadBrightness;

    // up arrow
    message.data1 = 0x68;
    message.data2 = LaunchKey::Brightness::P50;
    midiOut->SendMidiMessage(message);

    // down arrow
    message.data1 = 0x69;
    message.data2 = LaunchKey::Brightness::P75;
    midiOut->SendMidiMessage(message);

    // record button
    message.data1 = 0x75;
    message.data2 = LaunchKey::Brightness::P100;
    midiOut->SendMidiMessage(message);

    message.status = MidiStatus::NON;

    init = true;
}

void
Application::MIDILoop(void)
{
    std::optional<MidiMessage>  messageIn = midiIn->GetMidiMessage();
    if (messageIn)
    {
        if (CompareMidiMessage(*messageIn, LaunchKey::Play))
            isPlaying = false;

        if (!messageIn->data2)
        {
            std::cout   << "channel: " << messageIn->channel
                        << " status: " << messageIn->status
                        << " data1: " << messageIn->data1.value() << std::endl;
        }
        else
        {
            std::cout   << "channel: " << messageIn->channel
                        << " status: " << messageIn->status
                        << " data1: " << messageIn->data1.value()
                        << " data2: " << messageIn->data2.value() << std::endl;
        }
    }

    if (mode == 1)
        ColorPicker(messageIn);

    else if (mode == 2)
        Mario();

    // Disco
    else if (mode == 3)
        Disco();
    
    // Touch
    else if (mode == 4)
        Touch(messageIn);

    else if (mode == 8)
        Brightness();
}

void
Application::Run(void)
{
    do
    {
        MIDILoop();
    }
    while (isPlaying);

    {
        MidiMessage colMessage = LaunchKey::DrumPadColor;
        colMessage.data2   =   0;
        for (int i = 0; i < 4; ++i)
        {
            colMessage.data1   =   LaunchKey::DrumPads::DP1 + i;
            midiOut->SendMidiMessage(colMessage);
        }

        for (int i = 0; i < 4; ++i)
        {
            colMessage.data1   =   LaunchKey::DrumPads::DP5 + i;
            midiOut->SendMidiMessage(colMessage);
        }

        for (int i = 0; i < 4; ++i)
        {
            colMessage.data1   =   LaunchKey::DrumPads::DP9 + i;
            midiOut->SendMidiMessage(colMessage);
        }

        for (int i = 0; i < 4; ++i)
        {
            colMessage.data1   =   LaunchKey::DrumPads::DP13 + i;
            midiOut->SendMidiMessage(colMessage);
        }
    }

    {
        MidiMessage colMessage = LaunchKey::SessionPadColor;
        colMessage.data2   =   0;
        for (int i = 0; i < 8; ++i)
        {
            colMessage.data1   =   LaunchKey::SessionPads::SP1 + i;
            midiOut->SendMidiMessage(colMessage);
        }
        for (int i = 0; i < 8; ++i)
        {
            colMessage.data1   =   LaunchKey::SessionPads::SP9 + i;
            midiOut->SendMidiMessage(colMessage);
        }
    }

    MidiMessage message = LaunchKey::Play;
    message.channel = MidiChannel::CH2;
    message.data2 = LaunchKey::Brightness::P0;
    midiOut->SendMidiMessage(message);

    midiOut->SendMidiMessage(LaunchKey::DAWModeOff);
}

