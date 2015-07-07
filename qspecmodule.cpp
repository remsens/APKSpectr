#include "qspecmodule.h"


DEVICE_STATE QSpecModule::sendDevComand(const FT_HANDLE &devHandle, ServiceData *serviceData)
{
    DWORD bytes_written = 0;
    ft_status = FT_Write(devHandle, serviceData, sizeof(ServiceData), &bytes_written);
    if ((ft_status == FT_OK) && (bytes_written == sizeof(ServiceData)))
    {
        DWORD bytes_read = 0;
        ZeroMemory(serviceData, sizeof(ServiceData));
        ft_status = FT_Read(devHandle, serviceData, sizeof(ServiceData), &bytes_read);
        if ((ft_status == FT_OK) && (bytes_read == sizeof(ServiceData)))
        {
            return D_OK;
        }
        else {return E_DEVICE_READ;}
    } else {return E_DEVICE_WRITE;}
}

DEVICE_STATE QSpecModule::getDevId(const FT_HANDLE &devHandle, QString* devID)
{
    ServiceData service_data;
    ZeroMemory(&service_data, sizeof(ServiceData));
    service_data.m1 = 0xA5;
    service_data.m2 = 0x5A;
    service_data.comand = GET_ID;
    service_data.controll_sum = 256 - calcControllSum((void*)&service_data, SENT_COMAND_SIZE);

    DEVICE_STATE d_state = sendDevComand(devHandle, &service_data);
    if (d_state == D_OK)
    {

        if (calcControllSum((void*)&service_data, RECIVED_COMAND_SIZE) == 0)
        {
            devID->clear();
            devID->append(QString::number(service_data.parameter_value1));
            return D_OK;
        } else {return E_CSUM;}
    } else {return d_state;}

}
DEVICE_STATE QSpecModule::getSpectrumData(const FT_HANDLE &devHandle, SpectrumData *spectrumData)
{
    ServiceData service_data;
    ZeroMemory(&service_data, sizeof(ServiceData));
    service_data.m1 = 0xA5;
    service_data.m2 = 0x5A;
    service_data.comand = GET_SPEC;
    service_data.controll_sum = 256 - calcControllSum((void*)&service_data, SENT_COMAND_SIZE);

    DWORD bytes_written = 0;
    ft_status = FT_Write(devHandle, (void*)&service_data, sizeof(ServiceData), &bytes_written);
    if ((ft_status == FT_OK) && (bytes_written == sizeof(ServiceData)))
    {
            DWORD bytes_read = 0;
            ft_status = FT_Read(devHandle, (void*)spectrumData, sizeof(SpectrumData), &bytes_read);
            if (ft_status == FT_OK)
            {
                if (bytes_read == sizeof(SpectrumData))
                {
                    return D_OK;
                }
                else {return E_DEVICE_READ;}
            }else {return E_DEVICE_READ;}
    }else {return E_DEVICE_WRITE;}
}

DEVICE_STATE QSpecModule::getMirrorPosition(const FT_HANDLE &devHandle, DWORD *mirrorPosition)
{
    ServiceData service_data;
    ZeroMemory(&service_data, sizeof(ServiceData));
    service_data.m1 = 0xA5;
    service_data.m2 = 0x5A;
    service_data.comand = GET_POS;
    service_data.controll_sum = 256 - calcControllSum((void*)&service_data, SENT_COMAND_SIZE);

    DEVICE_STATE d_state = sendDevComand(devHandle, &service_data);
    if (d_state == D_OK)
    {
        if (calcControllSum((void*)&service_data, RECIVED_COMAND_SIZE) == 0)
        {
            if(service_data.comand == GET_POS)
            {
                *mirrorPosition = service_data.parameter_value1;
                return D_OK;
            } else {return E_GET_MIRROR;}
        } else {return E_CSUM;}
    } else {return d_state;}
}

DEVICE_STATE QSpecModule::getDeviceState()
{
    return dev_state;
}
DEVICE_STATE QSpecModule::isDevButtonPressed(const FT_HANDLE &devHandle, bool &buttonPressed)
{
    byte button_mode;
    ft_status = FT_GetBitMode(devHandle, &button_mode);
    if (ft_status == FT_OK)
    {
        button_mode = button_mode & 0x6;
        if (button_mode == false)
        {
            buttonPressed = true;
        }else {buttonPressed = false;}
        return D_OK;
    }else {return  E_BUTTON;}
}

DEVICE_STATE QSpecModule::setDevClock(const FT_HANDLE &devHandle)
{
    ServiceData service_data;
    ZeroMemory(&service_data, sizeof(ServiceData));
    QTime current_time = QTime::currentTime();
    DWORD c_time = ((current_time.hour() * 3600 + current_time.minute()*60 + current_time.second()) * 1000 + current_time.msec()) * 31.25;
    service_data.m1 = 0xA5;
    service_data.m2 = 0x5A;
    service_data.comand = SET_CLOCK;
    service_data.parameter_value1 = c_time & 0x0000ffff;
    service_data.parameter_value2 = (c_time & 0xffff0000) >> 16;
    service_data.controll_sum = 256 - calcControllSum((void*)&service_data, SENT_COMAND_SIZE);
    DEVICE_STATE d_state = sendDevComand(devHandle, &service_data);
    if (d_state == D_OK)
    {
        if (calcControllSum((void*)&service_data, RECIVED_COMAND_SIZE) == 0)
        {
            if(service_data.comand == SET_CLOCK)
            {
                return D_OK;
            } else {return E_SET_DCLOCK;}
        } else {return E_CSUM;}
    } else {return d_state;}
}

DEVICE_STATE QSpecModule::setDevExposition(const FT_HANDLE &devHandle, const DWORD &devExposition)
{
    ServiceData service_data;
    ZeroMemory(&service_data, sizeof(ServiceData));
    service_data.m1 = 0xA5;
    service_data.m2 = 0x5A;
    service_data.comand = SET_EXPO;
    service_data.parameter_value1 = devExposition;
    service_data.controll_sum = 256 - calcControllSum((void*)&service_data, SENT_COMAND_SIZE);

    DEVICE_STATE d_state = sendDevComand(devHandle, &service_data);
    if (d_state == D_OK)
    {
        if (calcControllSum((void*)&service_data, RECIVED_COMAND_SIZE) == 0)
        {
            if(service_data.comand == SET_EXPO)
            {
                return D_OK;
            } else {return E_SET_DEXPO;}
        } else {return E_CSUM;}
    } else {return d_state;}
}

DEVICE_STATE QSpecModule::setMirrorPosition(const FT_HANDLE &devHandle, const DWORD &mirrorPosition)
{
    ServiceData service_data;
    ZeroMemory(&service_data, sizeof(ServiceData));
    service_data.m1 = 0xA5;
    service_data.m2 = 0x5A;
    service_data.comand = SET_POS;
    service_data.parameter_value1 = mirrorPosition;
    service_data.controll_sum = 256 - calcControllSum((void*)&service_data, SENT_COMAND_SIZE);

    DEVICE_STATE d_state = sendDevComand(devHandle, &service_data);
    if (d_state == D_OK)
    {
        if (calcControllSum((void*)&service_data, RECIVED_COMAND_SIZE) == 0)
        {
            if(service_data.comand == SET_POS)
            {
                return D_OK;
            } else {return E_SET_MIRROR;}
        } else {return E_CSUM;}
    } else {return d_state;}
}

DEVICE_STATE QSpecModule::checkDevicePresent()
{
    DWORD     device_count = 0;
    FT_DEVICE_LIST_INFO_NODE *device_info_list;
    ft_status = FT_CreateDeviceInfoList(&device_count);
    if (ft_status == FT_OK)
    {
        device_info_list = (FT_DEVICE_LIST_INFO_NODE*)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*device_count);
        ft_status = FT_GetDeviceInfoList(device_info_list, &device_count);
        if (ft_status == FT_OK)
        {
            for (DWORD i=0; i < device_count; i++)
            {
                qDebug () << ftdi_serial << " " << device_info_list[i].SerialNumber << " " << i;
                if (QString::fromLatin1(device_info_list[i].SerialNumber) == ftdi_serial)
                {
                    dev_number = i;
                    free(device_info_list);
                    return D_OK;
                }
            }
            free(device_info_list);
            return E_NOAPPROPIATE_DEVICE;
        }else
        {
            free(device_info_list);
            return E_NODEVICE_INFO;
        }

    }else
    {
        free(device_info_list);
        return E_ZERO_DEVICES;
    }
}

DEVICE_STATE QSpecModule::connectDevice(FT_HANDLE* deviceHandle)
{
    ft_status = FT_Open(dev_number, deviceHandle);
    if (ft_status == FT_OK)
    {
        return D_OK;
    }else {return E_MFOPEN;}

}

DEVICE_STATE QSpecModule::setupDevice(const FT_HANDLE &deviceHandle)
{

//    ft_status = FT_ResetDevice(deviceHandle);
//    if (ft_status != FT_OK) {return E_RESET;}
    ft_status = FT_SetBitMode(deviceHandle, 0x00, 0x00);
    if (ft_status != FT_OK) {return E_RESET;}
    ft_status = FT_SetBaudRate(deviceHandle, FT_BAUD_921600);
//    ft_status = FT_SetBaudRate(deviceHandle, 1000000);
    if (ft_status != FT_OK) {return E_SET_BAUD;}
    ft_status = FT_SetDataCharacteristics(deviceHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
    if (ft_status != FT_OK) {return E_SET_CHARACT;}
//    ft_status = FT_SetBitMode(deviceHandle, 0x00, 0x20);
//    if (ft_status != FT_OK) {return E_SET_BITMODE;}
    ft_status = FT_SetTimeouts(deviceHandle, 500, 100);
    if (ft_status != FT_OK) {return E_SET_TIMEOUT;}
    return D_OK;
}

DEVICE_STATE QSpecModule::shootSpectrum(const FT_HANDLE &devHandle, SpectrumData *spectrumData, const int expo)
{
    DEVICE_STATE d_state;
    DWORD exp;
    if (expo == 0)
    {
        current_module_state = MODULE_SET_EXPO;
        d_state = setDevExposition(devHandle, 15); //устанавливаем 13мс на экспозицию
        if (d_state != D_OK)
        {
            logError();
            return d_state;
        }
        current_module_state = MODULE_GET_SPECTRUM;
        SpectrumData test_data;
        ZeroMemory(&test_data, sizeof(SpectrumData));
        d_state = getSpectrumData(devHandle, &test_data);
        if (d_state != D_OK)
        {
            logError();
            return d_state;
        }
        current_module_state = MODULE_SET_EXPO;
        exp = calcOptimalExpo(&test_data);
    } else {
        exp = expo;//расчитываем нужную экспозицию и устанавливаем
    }
    d_state = setDevExposition(devHandle, exp);
    if (d_state != D_OK)
    {
        logError();
        return d_state;
    }
    current_module_state = MODULE_GET_SPECTRUM;
    ZeroMemory(spectrumData, sizeof(SpectrumData));
    d_state = getSpectrumData(devHandle, spectrumData);
    if (d_state != D_OK)
    {
        logError();
        return d_state;
    }
    return D_OK;

}

int QSpecModule::calcOptimalExpo(SpectrumData *spectrumData)
{

    int spectrum_size = sizeof(spectrumData->spectrum)/sizeof(short int);
    short int max_value = spectrumData->spectrum[0];
    for (int i=1; i < spectrum_size; i++)
    {
        if (max_value < spectrumData->spectrum[0])
        {
            max_value = spectrumData->spectrum[0];
        }
    }

    DWORD new_exposition = 0;
    if (max_value < 2000)
    {
        new_exposition = ((spectrumData->header.exposition) * 3000) / max_value;
    } else {new_exposition = spectrumData->header.exposition;}

    return new_exposition;
}

char QSpecModule::calcControllSum(void* data, const int &dataCount)
{
    char* cs = (char*)data;
    char c=0;
    for (int i=0; i <=dataCount; i++)
    {
        c += *cs++;
    }
    return c;
}

QString QSpecModule::getLastError()
{
    return QString::number(ft_status);
}

void QSpecModule::logState()
{

}

void QSpecModule::logError()
{
    qDebug() << getLastError() << DEVICE_ERRORS_DESCRIPTION[dev_state].c_str() << " "
                                    << MODULE_STATE_DESCRIPTION[current_module_state].c_str();
    byte dat[32];
    ZeroMemory(dat, 32);
    DWORD bytes_written = 0;
    ft_status = FT_Write(dev_handle, dat, 32, &bytes_written);
    qDebug() << getLastError() << DEVICE_ERRORS_DESCRIPTION[dev_state].c_str() << " "
                                    << MODULE_STATE_DESCRIPTION[current_module_state].c_str() << "  " << bytes_written;
}

QSpecModule::QSpecModule()
{
    is_button_pressed = false;
    is_dev_shooting   = false;
    is_device_dms     = false;
}

QSpecModule::QSpecModule(const QString &ftdiSerial):QSpecModule()
{
    ftdi_serial = ftdiSerial;
    if (ftdi_serial == "A703OLZY") {is_device_dms = true;}
    dev_state = checkDevicePresent();
}

QSpecModule::~QSpecModule()
{
    if (dev_handle != NULL)
    {
        FT_Close(dev_handle);
    }
}

void QSpecModule::initDevice()
{
    dev_handle = NULL;
    dev_state = connectDevice(&dev_handle);
    if (dev_state != D_OK)
    {
        logError();
    }
    dev_state = setupDevice(dev_handle);
    if (dev_state != D_OK)
    {
        logError();
    }

    current_module_state = MODULE_GET_ID;
    dev_id = "";
    dev_state = getDevId(dev_handle, &dev_id);
    if (dev_state != D_OK)
    {
        logError();
    }

    current_module_state = MODULE_SET_CLOCK;
    dev_state = setDevClock(dev_handle);
    if (dev_state != D_OK)
    {
        logError();
    }

    current_module_state = MODULE_SET_EXPO;
    dev_state = setDevExposition(dev_handle, 20);
    if (dev_state != D_OK)
    {
        logError();
    }



}

void QSpecModule::startShoot()
{
    is_dev_shooting = true;
    while (is_dev_shooting)
    {
        SpectrumData spectrum_data;
        dev_state = shootSpectrum(dev_handle, &spectrum_data);
        if (dev_state != D_OK)
        {
            logError();
        }
        emit onCompletePass(&spectrum_data);
    }
}

void QSpecModule::stopShoot()
{
    is_dev_shooting = false;
    emit onCompleteShoot();
}

void QSpecModule::singleShot(int expo)
{
    if (is_device_dms)
    {
            current_module_state = MODULE_SET_MIRROR;
            dev_state = setMirrorPosition(dev_handle,488);
            if (dev_state != D_OK)
            {
                logError();
            }
            Sleep(150);
            SpectrumData spectrum_data;
            dev_state = shootSpectrum(dev_handle, &spectrum_data, expo);
            if (dev_state != D_OK)
            {
                logError();
            }
            emit onCompletePass(&spectrum_data);
            current_module_state = MODULE_SET_MIRROR;
            dev_state = setMirrorPosition(dev_handle,965);
            if (dev_state != D_OK)
            {
               logError();
            }
            Sleep(150);
            ZeroMemory(&spectrum_data, sizeof(SpectrumData));
            dev_state = shootSpectrum(dev_handle, &spectrum_data, expo);
            if (dev_state != D_OK)
            {
                logError();
            }
            emit onCompletePass(&spectrum_data);
     }
     else
    {
        SpectrumData spectrum_data;
        dev_state = shootSpectrum(dev_handle, &spectrum_data, expo);
        if (dev_state != D_OK)
        {
            logError();
        }
        emit onCompletePass(&spectrum_data);
    }

}




