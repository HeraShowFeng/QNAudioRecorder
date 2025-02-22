﻿#ifndef QN_MICROPHONE_MANAGER_H
#define QN_MICROPHONE_MANAGER_H

#include "qn_rtc_base.h"
#include "qn_audio_recorder_interface.h"
using namespace std;
namespace qiniu {

#define VOLUMEMAX   32767
#define VOLUMEMIN   -32768

#ifndef core_min
#define core_min(a, b)  ((a) < (b) ? (a) : (b))
#endif

  enum AudioDeviceType {
    kAudioDeviceInvalid = -1,
    kAudioDevicePlayout,
    kAudioDeviceRecord,
  };

  class QNMicrophoneManager
    : public AudioDeviceDataObserver
    , public QNAudioRecorder {
   public:
     static QNMicrophoneManager* ObtainMicrophoneSessionInterface();
     static void DestroyMicrophoneSessionInterface(QNMicrophoneManager* ptr);
     virtual ~QNMicrophoneManager();

    // implements of AudioDeviceDataObserver
    virtual void OnCaptureData(
        const void* audio_samples,
        const size_t num_samples,
        const size_t bytes_per_sample,
        const size_t num_channels,
        const uint32_t samples_per_sec);

    virtual void OnRenderData(
        const void* audio_samples,
        const size_t num_samples,
        const size_t bytes_per_sample,
        const size_t num_channels,
        const uint32_t samples_per_sec);

    // implements of QNMicrophoneInterface
    virtual int32_t GetAudioRecordingDeviceCount();
    virtual QNAudioDeviceInfo& GetAudioRecordingDeviceInfo(int32_t index);
    virtual int32_t SetAudioRecordingDevice(int32_t index);
    virtual int32_t Start(QNAudioVolumeCallback* callback);
    virtual int32_t Stop();

   private:
    QNMicrophoneManager();
    QNMicrophoneManager(const QNMicrophoneManager&) = delete;
    QNMicrophoneManager operator = (const QNMicrophoneManager&) = delete;

    int32_t GetAudioRecordingDeviceCountInternal();
    QNAudioDeviceInfo& GetAudioRecordingDeviceInfoInternal(int32_t index);
    int32_t GetAudioPlaybackDeviceCountInternal();
    QNAudioDeviceInfo GetAudioPlaybackDeviceInfoInternal(int32_t index);
    int32_t SetAudioRecordingDeviceInternal(int32_t index);
    int32_t SetAudioPlaybackDeviceInternal(int32_t index);
    int32_t StartRecordingInternal();
    int32_t StopRecordingInternal();
    int32_t GetAudioVolumeInternal(AudioDeviceType device_type, uint32_t* volume);
    double ProcessAudioLevel(const int16_t* data, const int32_t& data_size, int32_t offset);
    std::wstring Utf8ToWstring(const std::string& utf8_str);
    std::string  WstringToUtf8(const std::wstring& wstr);
    std::string  UnicodeToUtf(std::wstring str);
    std::wstring UtfToUnicode(std::string str);
    int32_t GetDefaultDeviceId(bool is_recording, string& device_id);
   private:
     static QNMicrophoneManager*                    global_microphone_session_;
     QNAudioVolumeCallback*                         record_volume_callback_;
     std::shared_ptr<rtc::Thread>                   worker_thread_ptr_;
     rtc::scoped_refptr<webrtc::AudioDeviceModule>  adm_ptr_;
     std::vector<QNAudioDeviceInfo>                 audio_recording_device_info_vec_;
     std::vector<QNAudioDeviceInfo>                 audio_playback_device_info_vec_;
     std::chrono::steady_clock::time_point          last_callback_time_;
  };
}

#endif // QN_MICROPHONE_MANAGER_H
