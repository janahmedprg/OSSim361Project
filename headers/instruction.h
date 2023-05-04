enum InstructionType {
    JA,
    DReq,
    DRel,
    Display,
};

struct JobArrival {
    int jobNumber;
    int memoryRequirement;
    int devicesRequirement;
    int burstTime;
    int priority;
};

struct DeviceRequest {
    int jobNumber;
    int deviceNumber;
};

struct DeviceRelease {
    int jobNumber;
    int deviceNumber;
};

struct Instruction {
    enum InstructionType type;
    int time;
    union {
        struct JobArrival jobArrival;
        struct DeviceRequest deviceRequest;
        struct DeviceRelease deviceRelease;
    } data;
};