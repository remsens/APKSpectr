#ifndef QSPECMODULE_H
#define QSPECMODULE_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QTime>
#include <vector>
#include <ftd2xx.h>
#include <windows.h>
#include <algorithm>
#include <math.h>
#define SENT_COMAND_SIZE    9
#define RECIVED_COMAND_SIZE  10
#define TICK_TIME 6.1273336524652943992340832934418

using namespace std;


enum DEVICE_COMAND {NOP,GET_ID,SET_EXPO,GET_EXP, GET_SPEC,SET_CLOCK, GET_CLOCK, SET_POS, GET_POS
                    };
enum DEVICE_STATE  {  D_OK                 // = 0x00
                    , D_READING            // = 0x02
                    , D_WRITING            // = 0x04
                    , D_MAILFUNCTION       // = 0x08
                    , D_CONNECTED          // = 0x0f
                    , D_DISCONNECTED
                    , D_PRESENT
                    , D_SETUPED
                    , E_NOAPPROPIATE_DEVICE
                    , E_ZERO_DEVICES
                    , E_NODEVICE_INFO
                    , E_SET_TIMEOUT
                    , E_MFOPEN
                    , E_SET_BAUD
                    , E_SET_CHARACT
                    , E_SET_BITMODE
                    , E_DEVICE_WRITE
                    , E_DEVICE_READ
                    , E_CSUM
                    , E_SET_DCLOCK
                    , E_SET_DEXPO
                    , E_BUTTON
                    , E_RESET
                    , E_SET_MIRROR
                    , E_GET_MIRROR
                   };
enum MODULE_STATE {   MODULE_CONNECTING,
                      MODULE_SET_CLOCK,
                      MODULE_SET_EXPO,
                      MODULE_GET_ID,
                      MODULE_GET_SPECTRUM,
                      MODULE_CALC_MAX,
                      MODULE_CALC_CS,
                      MODULE_GET_ANGLE,
                      MODULE_GET_TEMP,
                      MODULE_SET_MIRROR,
                      MODULE_GET_MIRROR};


const std::string DEVICE_ERRORS_DESCRIPTION[] = {"Device OK", "Device reading","Device writing", "Device in mailfunction state",
                                     "Device connected", "Device disconnected", "Device present onboard",
                                     "Device setuped", "No appropiate device in system","No one device ins sytem",
                                     "No device information", "Setting timeout error", "Device open error",
                                     "Setting baud rate error", "Setting characteristic error", "Setting bitmode error",
                                     "Error write", "Error read", "Error control sum", "Error setting device clock",
                                     "Error setting exposition", "Device button mailfunction", "Error reseting device",
                                                 "Error setting mirror", "Error getting mirror"};
const std::string MODULE_STATE_DESCRIPTION[] = { "Module connecting", "Setting dev clock", "Setting expo", "Getting id", "getting spectrum",
                                                 "Callculating maximum spectrum", "Callculating controll summ",
                                                  "Getting angles", "Getting temp", "Setting mirror", "Getting mirror"};



#pragma pack(push,1)
struct ServiceData
{
    byte  m1;
    byte  comand;
    WORD  parameter_value1;
    WORD  parameter_value2;
    DWORD parameter_value3;
    byte  controll_sum;
    byte  m2;
};
struct SpectrumHeader
{
    WORD  exposition;
    WORD  xpos;
    DWORD time;
    byte  offset;
    byte  mode;
};

struct SpectrumData
{
    SpectrumHeader  header;
    WORD            dummy1[16];
    short int       black1[13];
    WORD            dummy2[3];
    short int       spectrum[3648];
    WORD            dummy[16];
    byte            controll_sum;
};


struct SpectrumRecord
{
    vector<SpectrumData>   spectrum_data;
    vector<SpectrumHeader> spectrum_header;
    int                    spectrum_count;
};
#pragma pack(pop)

class QSpecModule: public QObject
{
    Q_OBJECT


     int        exposition;
     double     x_angle;
     double     y_angle;
     double     dev_temperature;
     FT_HANDLE  dev_handle;
     FT_STATUS  ft_status;
     QString    dev_id;
     DWORD      dev_number;
     bool       is_dev_angleFNC;
     bool       is_button_pressed;
     bool       is_dev_shooting;
     bool       is_mirror_black;
     bool       is_device_dms;
     QString    ftdi_serial;


     DEVICE_STATE dev_state;
     MODULE_STATE current_module_state;

     QThread    spec_module_thread;


     DEVICE_STATE sendDevComand(const FT_HANDLE &devHandle, ServiceData* serviceData);
     DEVICE_STATE getDevId(const FT_HANDLE& devHandle, QString *devID);
     DEVICE_STATE getSpectrumData(const FT_HANDLE& devHandle, SpectrumData* spectrumData);
     DEVICE_STATE getMirrorPosition(const FT_HANDLE & devHandle, DWORD *mirrorPosition);
     //-----------------------------------------------------------------------------------
     DEVICE_STATE isDevButtonPressed(const FT_HANDLE& devHandle, bool &buttonPressed);
     //-----------------------------------------------------------------------------------
     DEVICE_STATE setDevClock(const FT_HANDLE& devHandle);
     DEVICE_STATE setDevExposition(const FT_HANDLE& devHandle, const DWORD &devExposition);
     DEVICE_STATE setMirrorPosition(const FT_HANDLE & devHandle, const DWORD &mirrorPosition);
     //-----------------------------------------------------------------------------------
     DEVICE_STATE checkDevicePresent();
     DEVICE_STATE connectDevice(FT_HANDLE *deviceHandle);
     DEVICE_STATE setupDevice(const FT_HANDLE &deviceHandle);
     //-----------------------------------------------------------------------------------
     DEVICE_STATE shootSpectrum(const FT_HANDLE &devHandle, SpectrumData* spectrumData, const int expo = 0);
     int          calcOptimalExpo(SpectrumData *spectrumData);
     //-----------------------------------------------------------------------------------
     int     calcAverageExposition(const SpectrumRecord& spectrumRecord);
     char    calcControllSum(void* data, const int& dataCount);

     QString getLastError();
     void    logState();
     void    logError();

public:

    QSpecModule();
    QSpecModule(const QString &ftdiSerial);
    ~QSpecModule();
    DEVICE_STATE getDeviceState();

public slots:
    void initDevice();
    void startShoot();
    void stopShoot();
    void singleShot(int expo);

signals:
    void onCompletePass(SpectrumData* spectrumData);
    //void onCompletePass(QDeviceData*);
    void onButtonPressed();
    void onCompleteShoot();



};

#endif // QSPECMODULE_H
