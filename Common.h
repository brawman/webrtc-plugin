#pragma once

#include "Config.h"
#include "Debug.h"
#include "DoubangoAudioDeviceModule.h"

#include "webrtc/api/peerconnectioninterface.h"
#include "webrtc/api/mediaconstraintsinterface.h"
#include "webrtc/api/mediastreaminterface.h"
#include "webrtc/base/thread.h"

class Buffer;
class ExErrorMessage;
class ExRTCError;
class ExMediaStream;
class ExMediaStreamConstraints;
class RTCMediaConstraints;
class ExRTCSessionDescription;
class ExRTCIceCandidate;
class ExRTCPeerConnectionIceEvent;
class ExMediaStreamEvent;
class ExRTCDataChannelEvent;
class ExRTCTrackEvent;
class ExRTCDTMFToneChangeEvent;
class ExRTCStats;
class ExRTCStatsReport;
class ExMessageEvent;

typedef void* VoidPtr;
typedef VoidPtr DtmfSenderInterfacePtr,
VideoTrackInterfacePtr,
MediaStreamInterfacePtr,
MediaStreamTrackInterfacePtr,
VideoRendererPtr,
DataChannelInterfacePtr;

#define RTC_MAX_ARGS_PARAMS 10

#define RTC_nullable_ushort_null -1
typedef int RTC_nullable_ushort;

// Errors
#define RTC_ConstraintNotSatisfiedError		"ConstraintNotSatisfiedError"
#define RTC_PermissionDeniedError			"PermissionDeniedError"
#define RTC_OperationError					"OperationError"

#define RTC_SAFE_RELEASE(ppObj) { if ((ppObj) && *(ppObj)) { (*(ppObj))->Release(); *(ppObj) = NULL; } }
#define RTC_SAFE_RELEASE_OBJECT(ppObj) { if ((ppObj) && *(ppObj)) { (*(ppObj))->ReleaseObject(); *(ppObj) = NULL; } }
#define RTC_SAFE_FREE(ppObj) { if ((ppObj) && *(ppObj)) { free(*(ppObj)); *(ppObj) = NULL; } }

// In CHECK_HR(x) When (x) is a function it will be executed twice when used in "WE_DEBUG_ERROR(x)" and "If(x)"
#define RTC_CHECK_HR_BAIL(x) { HRESULT __hr__ = (x); if (FAILED(__hr__)) { RTC_DEBUG_ERROR("Operation Failed (%08x)", __hr__); goto bail; } }
#define RTC_CHECK_HR_RETURN(x) { HRESULT __hr__ = (x); if (FAILED(__hr__)) { RTC_DEBUG_ERROR("Operation Failed (%08x)", __hr__); return __hr__; } }
#define RTC_CHECK_HR_NOP(x) { HRESULT __hr__ = (x); if (FAILED(__hr__)) { RTC_DEBUG_ERROR("Operation Failed (%08x)", __hr__); } }

enum TrackTypeFlags {
	TrackTypeFlagsNone = 0x00,
	TrackTypeFlagsAudio = (0x01 << 0),
	TrackTypeFlagsVideo = (0x01 << 1),
	TrackTypeFlagsAudioVideo = TrackTypeFlagsAudio | TrackTypeFlagsVideo,
	TrackTypeFlagsAll = 0xFF
};

// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Typed_arrays
enum ArrayType
{
	ArrayType_None,
	ArrayType_Int8Array, //	1	8 - bit twos complement signed integer	signed char
	ArrayType_Uint8Array, //	1	8 - bit unsigned integer	unsigned char
	ArrayType_Uint8ClampedArray, //	1	8 - bit unsigned integer	unsigned char
	ArrayType_Int16Array, //	2	16 - bit twos complement signed integer	short
	ArrayType_Uint16Array, //	2	16 - bit unsigned integer	unsigned short
	ArrayType_Int32Array, //	4	32 - bit twos complement signed integer	int
	ArrayType_Uint32Array, //	4	32 - bit unsigned integer	unsigned int
	ArrayType_Float32Array, //	4	32 - bit IEEE floating point number	float
	ArrayType_Float64Array, //	8	64 - bit IEEE floating point number	double
};


struct RTCIceServer {
public:
	std::vector<std::string> urls;
	std::string username;
	std::string password;
	RTCIceServer(std::vector<std::string> _urls, std::string _username = std::string(""), std::string _password = std::string("")) {
		urls = _urls, username = _username, password = _password;
	}
};

struct RTCConfiguration {
public:
	std::vector<RTCIceServer>iceServers;
	RTCConfiguration() {
	}
	~RTCConfiguration() {
		iceServers.clear();
	}
};

// http://www.w3.org/TR/mediacapture-streams/#idl-def-NavigatorUserMediaSuccessCallback
typedef std::function<void(std::shared_ptr<ExMediaStream> stream)> NavigatorUserMediaSuccessCallback;

// http://www.w3.org/TR/mediacapture-streams/#idl-def-NavigatorUserMediaErrorCallback
typedef std::function<void(std::shared_ptr<ExErrorMessage> e)> NavigatorUserMediaErrorCallback;

typedef std::function<void()> FunctionCallbackVoid;
typedef std::function<void(std::shared_ptr<std::string> string)> FunctionCallbackString;
typedef std::function<void(std::shared_ptr<ExErrorMessage> e)> FunctionCallbackError; // DomError
typedef std::function<void(std::shared_ptr<ExRTCError> e)> FunctionCallbackRTCError; // RTCError
typedef std::function<void(std::shared_ptr<ExRTCSessionDescription> e)> FunctionCallbackSessionDescription;
typedef std::function<void(std::shared_ptr<ExRTCPeerConnectionIceEvent> e)> FunctionCallbackIceCandidate;
typedef std::function<void(std::shared_ptr<ExMediaStreamEvent> e)> FunctionCallbackStream;
typedef std::function<void(std::shared_ptr<ExRTCDataChannelEvent> e)> FunctionCallbackDataChannel;
typedef std::function<void(std::shared_ptr<ExRTCTrackEvent> e)> FunctionCallbackTrack;
typedef std::function<void(std::shared_ptr<ExRTCDTMFToneChangeEvent> e)> FunctionCallbackDTMFToneChange;
typedef std::function<void(std::shared_ptr<ExRTCStatsReport> e)> FunctionCallbackStatsReport;
typedef std::function<void(std::shared_ptr<ExMessageEvent> e)> FunctionCallbackMessage;

static const std::string kStringEmpty = "";

// https://www.w3.org/TR/mediacapture-streams/#idl-def-MediaStreamTrackState
static const char kMediaStreamTrackStateLive[] = "live";
static const char kMediaStreamTrackStateEnded[] = "ended";

// https://www.w3.org/TR/mediacapture-streams/#idl-def-MediaDeviceKind
static const char kMediaDeviceKindAudioInput[] = "audioinput";
static const char kMediaDeviceKindAudioOutput[] = "audiooutput";
static const char kMediaDeviceKindVideoInput[] = "videoinput";

// https://www.w3.org/TR/webrtc/#dom-rtcsdptype
static const char kRTCSdpTypeOffer[] = "offer";
static const char kRTCSdpTypePranswer[] = "pranswer";
static const char kRTCSdpTypeAnswer[] = "answer";
static const char kRTCSdpTypeRollback[] = "rollback";

// https://www.w3.org/TR/webrtc/#dom-rtcsignalingstate
static const char kSignalingStateStable[] = "stable";
static const char kSignalingStateHaveLocalOffer[] = "have-local-offer";
static const char kSignalingStateHaveRemoteOffer[] = "have-remote-offer";
static const char kSignalingStateHaveLocalPrAnswer[] = "have-local-pranswer";
static const char kSignalingStateHaveRemotePrAnswer[] = "have-remote-pranswer";
static const char kSignalingStateClosed[] = "closed";

// https://www.w3.org/TR/webrtc/#dom-rtcicegatheringstate
static const char kRTCIceGatheringStateNew[] = "new";
static const char kRTCIceGatheringStateGathering[] = "gathering";
static const char kRTCIceGatheringStateComplete[] = "complete";

// https://www.w3.org/TR/webrtc/#dom-rtciceconnectionstate
static const char kRTCIceConnectionStateNew[] = "new";
static const char kRTCIceConnectionStateChecking[] = "checking";
static const char kRTCIceConnectionStateConnected[] = "connected";
static const char kRTCIceConnectionStateCompleted[] = "completed";
static const char kRTCIceConnectionStateFailed[] = "failed";
static const char kRTCIceConnectionStateDisconnected[] = "disconnected";
static const char kRTCIceConnectionStateClosed[] = "closed";

// https://www.w3.org/TR/webrtc/#dom-rtcpeerconnectionstate
static const char kRTCPeerConnectionStateNew[] = "new";
static const char kRTCPeerConnectionStateConnecting[] = "connecting";
static const char kRTCPeerConnectionStateConnected[] = "connected";
static const char kRTCPeerConnectionStateDisconnected[] = "disconnected";
static const char kRTCPeerConnectionStateFailed[] = "failed";
static const char kRTCPeerConnectionStateClosed[] = "closed";

// https://www.w3.org/TR/webrtc/#dom-rtcprioritytype
static const char kRTCPriorityTypeVeryLow[] = "very-low";
static const char kRTCPriorityTypeLow[] = "low";
static const char kRTCPriorityTypeMedium[] = "medium";
static const char kRTCPriorityTypeHigh[] = "high";

// https://www.w3.org/TR/webrtc/#dom-rtcdatachannelstate
static const char kRTCDataChannelStateConnecting[] = "connecting";
static const char kRTCDataChannelStateOpen[] = "open";
static const char kRTCDataChannelStateClosing[] = "closing";
static const char kRTCDataChannelStateClosed[] = "closed";


// https://www.w3.org/TR/webrtc/#dom-rtcdatachannelinit
struct RTCDataChannelInit {
public:
	// "nullable_ushort" is used instead of "unsigned short" to emulate nulable values: "-1 == null" (same rule is used in WebRTC internals)
	bool ordered = true; //  boolean ordered = true;
	RTC_nullable_ushort maxPacketLifeTime = RTC_nullable_ushort_null; // unsigned short ? maxRetransmitTime = null;
	RTC_nullable_ushort maxRetransmits = RTC_nullable_ushort_null; // unsigned short ? maxRetransmits = null;
	std::string protocol = ""; // USVString protocol = "";
	bool negotiated = false; // boolean negotiated = false;
	RTC_nullable_ushort id = RTC_nullable_ushort_null; // [EnforceRange] unsigned short ? id = null;
	std::string priority = kRTCPriorityTypeLow; // RTCPriorityType priority = "low";
};

// https://www.w3.org/TR/mediacapture-streams/#idl-def-MediaConstraintSet
typedef std::pair< std::string, std::string> MediaConstraint;
typedef std::map<std::string, std::string> MediaConstraintSet;
struct MediaConstraintSets {
public:
	std::shared_ptr<MediaConstraintSet> ideal;
	std::shared_ptr<MediaConstraintSet> exact;
	MediaConstraintSets(std::shared_ptr<MediaConstraintSet> _ideal = nullptr, std::shared_ptr<MediaConstraintSet> _exact = nullptr) {
		ideal = _ideal;
		exact = _exact;
	}
};

template <typename T>
struct Sequence {
	std::vector<std::shared_ptr<T> > values;
	Sequence() {

	}
	void Clear() {
		values.clear();
	}
	void Add(std::shared_ptr<T> v) {
		values.push_back(v);
	}
	void AddSeq(Sequence<T>* seq) {
		if (seq) {
			for (size_t i = 0; i < seq->values.size(); ++i) {
				values.push_back(seq->values[i]);
			}
		}
	}
	~Sequence() {
		Clear();
	}
};

template <typename T>
class AutoLock {
public:
	explicit AutoLock(T* obj) : obj_(obj) { obj_->Enter(); }
	virtual ~AutoLock() { obj_->Leave(); }
protected:
	T* obj_;
};

struct BrowserObject {
public:
	virtual struct BrowserObject* RetainObject() = 0;
	virtual struct BrowserObject* ReleaseObject() = 0;
};

#define RTC_DECLARE_BROWSER_OBJECT_RETAIN_RELEASE() \
	virtual BrowserObject* RetainObject() override { \
		if (AddRef()) return dynamic_cast<BrowserObject*>(this); \
		return nullptr; \
	} \
	virtual BrowserObject* ReleaseObject() override { \
		if (Release()) return dynamic_cast<BrowserObject*>(this); \
		return nullptr; \
	}

#define RTC_DECLARE_BROWSER_OBJECT_RETAIN_RELEASE_INLINE() \
	private: \
	volatile long m_nRefCount = 1; \
	public: \
	struct BrowserObject* RetainObject() { \
		assert(m_nRefCount > 0); \
		InterlockedIncrement(&m_nRefCount); return this; \
	} \
struct BrowserObject* ReleaseObject() { \
	assert(m_nRefCount > 0); \
	if (InterlockedDecrement(&m_nRefCount)) return this; \
	delete this; return nullptr; \
}

struct BrowserCallback : public BrowserObject {
public:
	unsigned msgid;
	BrowserCallback(unsigned _msgid) {
		msgid = _msgid;
	}
	virtual unsigned GetMsgId() { return msgid; }
	virtual bool Invoke() = 0;
};

extern rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> GetPeerConnectionFactory();
extern rtc::scoped_refptr<webrtc::AudioDeviceModule> GetAudioDeviceModule();
extern rtc::Thread* GetWorkerThread();
extern void TakeFakePeerConnectionFactory();
extern void ReleaseFakePeerConnectionFactory();
extern rtc::scoped_refptr<RTCMediaConstraints> BuildConstraints(const MediaConstraintSets* constraints = NULL);
extern webrtc::MediaStreamInterface* BuildMediaStream(const ExMediaStream* stream);
extern void getUserMedia(const ExMediaStreamConstraints* constraints = NULL, NavigatorUserMediaSuccessCallback successCallback = nullptr, NavigatorUserMediaErrorCallback errorCallback = nullptr);
