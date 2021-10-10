#pragma once

#include <optional>
#include <memory>

#include <rtmidi/RtMidi.h>

enum MidiNote
{
    C_2 = 0, Db_2, D_2, Eb_2, E_2, F_2, Gb_2, G_2, Ab_2, A_2, Bb_2, B_2,
    C_1, Db_1, D_1, Eb_1, E_1, F_1, Gb_1, G_1, Ab_1, A_1, Bb_1, B_1,
    C0, Db0, D0, Eb0, E0, F0, Gb0, G0, Ab0, A0, Bb0, B0,
    C1, Db1, D1, Eb1, E1, F1, Gb1, G1, Ab1, A1, Bb1, B1,
    C2, Db2, D2, Eb2, E2, F2, Gb2, G2, Ab2, A2, Bb2, B2,
    C3, Db3, D3, Eb3, E3, F3, Gb3, G3, Ab3, A3, Bb3, B3,
    C4, Db4, D4, Eb4, E4, F4, Gb4, G4, Ab4, A4, Bb4, B4,
    C5, Db5, D5, Eb5, E5, F5, Gb5, G5, Ab5, A5, Bb5, B5,
    C6, Db6, D6, Eb6, E6, F6, Gb6, G6, Ab6, A6, Bb6, B6,
    C7, Db7, D7, Eb7, E7, F7, Gb7, G7, Ab7, A7, Bb7, B7,
    C8, Db8, D8, Eb8, E8, F8, Gb8, G8
};

enum MidiStatus
{
    NOF     =   0x80,
    NON     =   0x90,
    PKP     =   0xA0,
    CC      =   0xB0,
    PC      =   0xC0,
    CP      =   0xD0,
    PB      =   0xE0,
    CLK     =   0xF8,
    START   =   0xFA,
    CONT    =   0xFB,
    STOP    =   0xFC,
    ACTSENS =   0xFE,
    SYSRES  =   0xFF,
};

enum MidiChannel
{
    CH1 =   0x0,
    CH2 =   0x1,
    CH3 =   0x2,
    CH4 =   0x3,
    CH5 =   0x4,
    CH6 =   0x5,
    CH7 =   0x6,
    CH8 =   0x7,
    CH9 =   0x8,
    CH10 =   0x9,
    CH11 =   0xA,
    CH12 =   0xB,
    CH13 =   0xC,
    CH14 =   0xD,
    CH15 =   0xE,
    CH16 =   0xF
};

struct MidiMessage
{
    MidiChannel                 channel;
    MidiStatus                  status;
    std::optional<unsigned int> data1   =   std::nullopt;
    std::optional<unsigned int> data2   =   std::nullopt;
};

bool
CompareMidiMessage(MidiMessage messageA, MidiMessage messageB);

namespace LaunchKey
{
    const   MidiMessage DAWModeOn =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::NON,
        .data1      =   0x0C,
        .data2      =   0x7F
    };

    const   MidiMessage DAWModeOff =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::NON,
        .data1      =   0x0C,
        .data2      =   0x00
    };

    const   MidiMessage SessionPadColor = 
    {
        .channel    =   MidiChannel::CH1,
        .status     =   MidiStatus::NON,
        .data1      =   0x00,
        .data2      =   0x00
    };

    const   MidiMessage SessionPadColorFlash = 
    {
        .channel    =   MidiChannel::CH2,
        .status     =   MidiStatus::NON,
        .data1      =   0x00,
        .data2      =   0x00
    };

    const   MidiMessage SessionPadColorPulse = 
    {
        .channel    =   MidiChannel::CH3,
        .status     =   MidiStatus::NON,
        .data1      =   0x00,
        .data2      =   0x00
    };

    const   MidiMessage DrumPadColor = 
    {
        .channel    =   MidiChannel::CH10,
        .status     =   MidiStatus::NON,
        .data1      =   0x00,
        .data2      =   0x00
    };

    const   MidiMessage DrumPadColorFlash = 
    {
        .channel    =   MidiChannel::CH11,
        .status     =   MidiStatus::NON,
        .data1      =   0x00,
        .data2      =   0x00
    };

    const   MidiMessage DrumPadColorPulse = 
    {
        .channel    =   MidiChannel::CH12,
        .status     =   MidiStatus::NON,
        .data1      =   0x00,
        .data2      =   0x00
    };

    const   MidiMessage PadBrightness =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x00,
        .data2      =   0x60
    };

    const   MidiMessage PadModeToggle =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x03,
        .data2      =   0x07
    };

    const   MidiMessage PadModePC =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x03,
        .data2      =   0x08
    };

    const   MidiMessage PadModeDrum2 =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x03,
        .data2      =   0x06
    };

    const   MidiMessage PadModeCustom =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x03,
        .data2      =   0x05
    };

    const   MidiMessage PadModeDrum =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x03,
        .data2      =   0x01
    };

    const   MidiMessage PadModeSession =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x03,
        .data2      =   0x02
    };

    const   MidiMessage KnobModeVolume =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x09,
        .data2      =   0x01
    };

    const   MidiMessage KnobModeDevice =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x09,
        .data2      =   0x02
    };

    const   MidiMessage KnobModePan =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x09,
        .data2      =   0x03
    };

    const   MidiMessage KnobModeSendsA =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x09,
        .data2      =   0x04
    };

    const   MidiMessage KnobModeSendsB =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x09,
        .data2      =   0x05
    };

    const   MidiMessage KnobModeCustom =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x09,
        .data2      =   0x06
    };

    const   MidiMessage ArrUp   =
    {
        .channel    =   MidiChannel::CH1,
        .status     =   MidiStatus::CC,
        .data1      =   0x68,
        .data2      =   0x7F
    };

    const   MidiMessage ArrDown =
    {
        .channel    =   MidiChannel::CH1,
        .status     =   MidiStatus::CC,
        .data1      =   0x69,
        .data2      =   0x7F
    };

    const   MidiMessage ArrLeft =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x67,
        .data2      =   0x7F
    };

    const   MidiMessage ArrRight=
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x66,
        .data2      =   0x7F
    };

    const   MidiMessage Shift   =
    {
        .channel    =   MidiChannel::CH1,
        .status     =   MidiStatus::CC,
        .data1      =   0x6C,
        .data2      =   0x7F
    };

    const   MidiMessage Play    =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x73,
        .data2      =   0x7F
    };

    const   MidiMessage Record    =
    {
        .channel    =   MidiChannel::CH16,
        .status     =   MidiStatus::CC,
        .data1      =   0x75,
        .data2      =   0x7F
    };

    enum DrumPads
    {
        DP1 =   MidiNote::E1,
        DP2,
        DP3,
        DP4,
        DP5 =   MidiNote::C2,
        DP6,
        DP7,
        DP8,
        DP9 =   MidiNote::C1,
        DP10,
        DP11,
        DP12,
        DP13 =   MidiNote::Ab1,
        DP14,
        DP15,
        DP16,
    };

    enum SessionPads
    {
        SP1    =   MidiNote::C6,
        SP2,
        SP3,
        SP4,
        SP5,
        SP6,
        SP7,
        SP8,
        SP9    =   MidiNote::E7,
        SP10,
        SP11,
        SP12,
        SP13,
        SP14,
        SP15,
        SP16
    };

    enum Knob
    {
        K1    =   21,
        K2,
        K3,
        K4,
        K5,
        K6,
        K7,
        K8
    };

    // % of brightness -> 0, 25, 50, 75, 100%
    enum Brightness
    {
        P0      =   0x00,
        P25     =   0x20,
        P50     =   0x30,
        P75     =   0x40,
        P100    =   0x60
    };
}

class MidiIn
{
    private:

        // 16 channels - 120 controllers - value
        std::array<std::array<unsigned char, 120>, 16>   CC =   {};
        std::unique_ptr<RtMidiIn>   midiIn =   nullptr;
        std::vector<unsigned char>  message;
        std::vector<std::string>    ports;

    public:

        MidiIn();

        std::vector<std::string>&
        GetPorts(void);

        bool
        IsPortOpen(void);

        void
        OpenPort(std::uint8_t numPort);

        void
        ClosePort(void);

        std::optional<MidiMessage>
        GetMidiMessage(void);

        double
        GetCCValue(std::uint8_t numCh, std::uint8_t numCtrl);
};

class MidiOut
{
    private:

        std::unique_ptr<RtMidiOut>  midiOut =   nullptr;
        std::vector<unsigned char>  message;
        std::vector<std::string>    ports;

    public:

        MidiOut();

        std::vector<std::string>&
        GetPorts(void);

        bool
        IsPortOpen(void);

        void
        OpenPort(std::uint8_t numPort);

        void
        ClosePort(void);

        void
        SendMidiMessage(MidiMessage midiMessage);
};
