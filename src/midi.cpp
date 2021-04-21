#include <midi.hpp>

bool
CompareMidiMessage(MidiMessage messageA, MidiMessage messageB)
{
    return  messageA.channel        ==  messageB.channel        &&
            messageA.status         ==  messageB.status         &&
            messageA.data1.value()  ==  messageB.data1.value()  &&
            messageA.data2.value()  ==  messageB.data2.value()  ;
}

MidiIn::MidiIn()
{
    midiIn  =   std::make_unique<RtMidiIn>();
}

std::vector<std::string>&
MidiIn::GetPorts(void)
{
    ports.clear();
    std::uint8_t numPorts   =   midiIn->getPortCount();
    for (std::uint8_t i = 0; i < numPorts; ++i)
        ports.push_back(midiIn->getPortName(i));
    return ports;
}

bool
MidiIn::IsPortOpen(void)
{
    return midiIn->isPortOpen();
}

void
MidiIn::OpenPort(std::uint8_t numPort)
{
    unsigned int numPorts   =   midiIn->getPortCount();
    if (numPort < numPorts)
        midiIn->openPort(numPort);
    else
        midiIn->openPort(0);

    midiIn->ignoreTypes(false, false, false);
}

void
MidiIn::ClosePort(void)
{
    midiIn->closePort();
}

std::optional<MidiMessage>
MidiIn::GetMidiMessage(void)
{
    if (!midiIn->isPortOpen()) return std::nullopt;

    midiIn->getMessage(&message);
    if (message.size() == 0)
        return std::nullopt;

    MidiMessage midiMessage;
    if (message.at(0) < 0xF0)
    {
        midiMessage.status  =   MidiStatus(message.at(0) & 0xF0);
        midiMessage.channel =   MidiChannel(message.at(0) & 0x0F);
        if (message.size() > 1)
            midiMessage.data1   =   std::make_optional(message.at(1));
        if (message.size() > 2)
            midiMessage.data2   =   std::make_optional(message.at(2));

        if (midiMessage.status == MidiStatus::CC)
        {
            CC.at(midiMessage.channel).at(*midiMessage.data1)  =   *midiMessage.data2;
        }
    }
    else
    {
        if (message.at(0) == MidiStatus::CLK) // skip clock sync
            return std::nullopt;
    }
    return std::make_optional(midiMessage);
}

double
MidiIn::GetCCValue(std::uint8_t numCh, std::uint8_t numCtrl)
{
    std::uint8_t    val =   CC.at(numCh).at(numCtrl);
    return val / 127.0;
}

MidiOut::MidiOut()
{
    midiOut =   std::make_unique<RtMidiOut>();
}

std::vector<std::string>&
MidiOut::GetPorts(void)
{
    ports.clear();
    std::uint8_t numPorts   =   midiOut->getPortCount();
    for (std::uint8_t i = 0; i < numPorts; ++i)
        ports.push_back(midiOut->getPortName(i));
    return ports;
}

void
MidiOut::OpenPort(std::uint8_t numPort)
{
    unsigned int numPorts   =   midiOut->getPortCount();
    if (numPort < numPorts)
        midiOut->openPort(numPort);
    else
        midiOut->openPort(0);
}

void
MidiOut::ClosePort(void)
{
    midiOut->closePort();
}

bool
MidiOut::IsPortOpen(void)
{
    return midiOut->isPortOpen();
}

void
MidiOut::SendMidiMessage(MidiMessage midiMessage)
{
    if (!midiOut->isPortOpen()) return;
    message.clear();
    
    message.push_back(midiMessage.status + midiMessage.channel);
    if (midiMessage.data1)
        message.push_back(*midiMessage.data1);
    if (midiMessage.data2)
        message.push_back(*midiMessage.data2);

    if (midiOut)
        midiOut->sendMessage(&message);
}
