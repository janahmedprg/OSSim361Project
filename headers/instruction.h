enum InstructionType {
    JA,
    DReq,
    DRel,
    Dis
};

struct JobArrival {
    int time;
    int jobNumber;
    int memoryRequirement;
    int devicesRequirement;
    int burstTime;
    int priority;
};

struct DeviceRequest {
    int time;
    int jobNumber;
    int deviceNumber;
};

struct DeviceRelease {
    int time;
    int jobNumber;
    int deviceNumber;
};

struct Display {
    int time;
};

struct Instruction {
    enum InstructionType type;
    union {
        struct JobArrival jobArrival;
        struct DeviceRequest deviceRequest;
        struct DeviceRelease deviceRelease;
        struct Display display;
    } data;
};