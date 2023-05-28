#define NDIS61 1
#if (_MSC_VER >= 1915)
#define no_init_all deprecated
#endif

#include "Ntifs.h"
#include <ntddk.h>

#include <wdf.h>
#pragma warning(push)
#pragma warning(disable: 4201)

#include <fwpsk.h>

#pragma warning(pop)			




#include <fwpvi.h>				

#include <guiddef.h>			
#include <initguid.h>			

#include <fwpmk.h>				

#include "devguid.h"
#include <ndis.h>
#include <ntstrsafe.h>
#include <fwpvi.h>
#include<guiddef.h>
#include<ntifs.h>
#include <winapifamily.h>

#pragma comment(lib,"fwpkclnt.lib")

#pragma comment(lib,"wdmsec.lib")
#define MY_DEVICE_STRING L"\\Device\\WFPDevice"
#define MY_DOS_DEVICE_STRING L"\\DosDevices\\WFPDevice"



DEFINE_GUID(WFP_SAMPLE_ESTABLISHED_CALLOUT_V4_GUID, 0xd969fc67, 0x6fb2, 0x4504, 0x91, 0xce, 0xa9, 0x7c, 0x3c, 0x32, 0xad, 0x36); //callout ID
DEFINE_GUID(WFP_SAMPLE_SUB_LAYER_GUID, 0xed6a516a, 0x36d1, 0x4881, 0xbc, 0xf0, 0xac, 0xeb, 0x4c, 0x4, 0xc2, 0x1c);	//sub_layer ID


PDEVICE_OBJECT g_device = NULL;
HANDLE EngineHandle = NULL;
UINT32 RegCalloutId = 0, AddCalloutId;
UINT64 filterid = 0;

NTSTATUS NotifyCallback(FWPS_CALLOUT_NOTIFY_TYPE type, const GUID* filterkey, const FWPS_FILTER0* filter)
{
	UNREFERENCED_PARAMETER(filterkey);
	UNREFERENCED_PARAMETER(filter);

	switch (type)
	{
	case FWPS_CALLOUT_NOTIFY_ADD_FILTER:
		DbgPrint(" Add Filter");
		break;
	case FWPS_CALLOUT_NOTIFY_DELETE_FILTER:
		DbgPrint("Delete Filter");
		break;
	}


	return STATUS_SUCCESS;

}

//다음에 수행되는것은   지연콜백

NTSTATUS FlowDeleteCallback(UINT16 layerid, UINT32 calloutid, UINT64 flowcontext)
{

	UNREFERENCED_PARAMETER(layerid);
	UNREFERENCED_PARAMETER(calloutid);
	UNREFERENCED_PARAMETER(flowcontext);

	return STATUS_SUCCESS;


}


//필터 콜백 
NTSTATUS FilterCallback(const FWPS_INCOMING_VALUES* values, const FWPS_INCOMING_METADATA_VALUES* metavaules, void* layerdata, const void* context, const FWPS_FILTER0* filter, UINT64 flowcontext, FWPS_CLASSIFY_OUT* classifyout)
{

	UINT32 local_address = values->incomingValue[FWPS_FIELD_OUTBOUND_TRANSPORT_V4_IP_LOCAL_ADDRESS].value.uint32;
	UINT32 remote_address = values->incomingValue[FWPS_FIELD_OUTBOUND_TRANSPORT_V4_IP_REMOTE_ADDRESS].value.uint32;
	UINT16 local_port = values->incomingValue[FWPS_FIELD_OUTBOUND_TRANSPORT_V4_IP_LOCAL_PORT].value.uint16;
	UINT16 remote_port = values->incomingValue[FWPS_FIELD_OUTBOUND_TRANSPORT_V4_IP_REMOTE_PORT].value.uint16;


	UNREFERENCED_PARAMETER(metavaules);
	UNREFERENCED_PARAMETER(layerData);
	UNREFERENCED_PARAMETER(classifyContext);
	UNREFERENCED_PARAMETER(flowContext);
	UNREFERENCED_PARAMETER(filter);

	KdPrint(("PID is %ld --- Path is %S --- LocalIp is %u.%u.%u.%u --- RemoteIp is %u.%u.%u.%u \r\n",
		(ULONG)(metavaules->processId),
		(PWCHAR)(metavaules->processPath->data),
		(LocalIp >> 24) & 0xff, (LocalIp >> 16) & 0xff, (LocalIp >> 8) & 0xff, (LocalIp)& 0xff,
		(RemoteIp >> 24) & 0xff, (RemoteIp >> 16) & 0xff, (RemoteIp >> 8) & 0xff, (RemoteIp)& 0xff));

	//원하는걸로 재량것 바꿔서 사용 가능
	UINT16 http_port = 80;

	if (remote_port == http_port)
	{
		KdPrint(("black /\r\n"));
		classifyout->actionType = FWP_ACTION_BLOCK;

		return;
	}
	else
	{
		classifyout->actionType = FWP_ACTION_PERMIT;
		KdPrint(("Pass"));
	}

$end:

	classifyout->actionType = FWP_ACTION_PERMIT;				

}

//언로드시 사용
VOID UnInitWfp()
{

	if (EngineHandle != NULL)
	{
		if (filterid != 0)
		{
			FwpmFilterDeleteById(EngineHandle, filterid);
			FwpmSubLayerDeleteByKey(EngineHandle, &WFP_SAMPLE_SUB_LAYER_GUID);
		}

		if (AddCalloutId != 0)
		{
			FwpmCalloutDeleteById(EngineHandle, AddCalloutId);

		}


		if (RegCalloutId != 0)
		{
			FwpsCalloutUnregisterById(RegCalloutId);
		}

		FwpmEngineClose(EngineHandle);
	}

}

//unload함수
VOID Unload(PDRIVER_OBJECT DriverObject)
{


	UnInitWfp();
	IoDeleteDevice(g_device);
	KdPrint(("unload\r\n"));


}
VOID empty_evt_unload(WDFDRIVER Driver)
{
	UNREFERENCED_PARAMETER(Driver);
	return;
}

NTSTATUS WfpOpenEngine()
{
	FWPM_SESSION wdf_session;
	wdf_session.flags= FWPM_SESSION_FLAG_DYNAMIC;


	return FwpmEngineOpen(NULL,
		RPC_C_AUTHN_WINNT,		
		NULL,					
		&wdf_session,			
		&EngineHandle			
		);

}




NTSTATUS WfpRegisterCallout()
{

	FWPS_CALLOUT0 s_allout = { 0, };

	//함수 적용
	s_allout.calloutKey = WFP_SAMPLE_ESTABLISHED_CALLOUT_V4_GUID; 
	s_allout.flags = 0;
	s_allout.classifyFn = FilterCallback;						
	s_allout.notifyFn = NotifyCallback;							
	s_allout.flowDeleteFn = FlowDeleteCallback;				
	//call out 등록


	return FwpsCalloutRegister(g_device,								
		&s_allout,										
		&RegCalloutId);									
}


//Callout을 등록합시다
NTSTATUS WfpAddCallout()
{


	FWPM_CALLOUT callout = { 0, };

	callout.flags = 0;																
	callout.displayData.name = L"EstablishedCalloutName";							
	callout.displayData.description = L"EstablishedCalloutNames ";					
	callout.calloutKey = WFP_SAMPLE_ESTABLISHED_CALLOUT_V4_GUID;					
	callout.applicableLayer = FWPM_LAYER_ALE_AUTH_CONNECT_V4;						
	//자세한 내용은	



	return FwpmCalloutAdd
		(
		EngineHandle,					
		&callout,						
		NULL,							
		&AddCalloutId					
		);

}

NTSTATUS WfpAddSublayer()
{
	FWPM_SUBLAYER sublayer = { 0, };

	sublayer.displayData.name = L"Establicshedsublayername";
	sublayer.displayData.description = L"Establishedsublayernames";
	sublayer.subLayerKey = WFP_SAMPLE_SUB_LAYER_GUID;
	sublayer.weight = 65500; 

	return FwpmSubLayerAdd
		(
		EngineHandle,														
		&sublayer,
		NULL		
		);

}


NTSTATUS WfpAddFilter()
{

	FWPM_FILTER filter = { 0, };
	FWPM_FILTER_CONDITION condition[1] = { 0, };
	FWP_V4_ADDR_AND_MASK AddrandMask = { 0, };					
	filter.displayData.name = L"filterCalloutName";
	filter.displayData.description = L"FilterCalloutName";
	filter.layerKey = FWPM_LAYER_ALE_AUTH_CONNECT_V4;			
	filter.subLayerKey = WFP_SAMPLE_SUB_LAYER_GUID;
	filter.weight.type = FWP_EMPTY;								
	filter.numFilterConditions = 1;								
	filter.filterCondition = condition;							
	

	filter.action.type = FWP_ACTION_CALLOUT_TERMINATING;		

	filter.action.calloutKey = WFP_SAMPLE_ESTABLISHED_CALLOUT_V4_GUID;

	condition[0].fieldKey = FWPM_CONDITION_IP_LOCAL_PORT;		
	condition[0].matchType = FWP_MATCH_EQUAL;			
	
	condition[0].conditionValue.type = FWP_V4_ADDR_MASK;				
	condition[0].conditionValue.v4AddrMask = &AddrandMask;;				
	return FwpmFilterAdd
		(
		EngineHandle,											
		&filter,												
		NULL,													
		&filterid												
		);

}


NTSTATUS init_drvier_objects(PDRIVER_OBJECT driver_obj,
	UNICODE_STRING *registry_path,
	WDFDRIVER *driver,WDFDEVICE* device)
{

	NTSTATUS status = STATUS_SUCCESS;
	WDF_DRIVER_CONFIG config = { 0, };
	UNICODE_STRING device_name = { 0, };
	UNICODE_STRING device_symlink = { 0, };
	PWDFDEVICE_INIT device_init = NULL;


	RtlInitUnicodeString(&device_name, MY_DEVICE_STRING);
	RtlInitUnicodeString(&device_symlink, MY_DOS_DEVICE_STRING);


	WDF_DRIVER_CONFIG_INIT(&config, WDF_NO_EVENT_CALLBACK);
	config.DriverInitFlags = WdfDriverInitNonPnpDriver;
	config.EvtDriverUnload = empty_evt_unload;

	status = WdfDriverCreate(driver_obj, registry_path, WDF_NO_OBJECT_ATTRIBUTES, &config, driver);

	device_init = WdfControlDeviceInitAllocate(*driver, &SDDL_DEVOBJ_SYS_ALL_ADM_ALL);

	if (!device_init)
	{
		status = STATUS_INSUFFICIENT_RESOURCES;
		goto $end;
	}

	WdfDeviceInitSetDeviceType(device_init, FILE_DEVICE_NETWORK);
	WdfDeviceInitSetCharacteristics(device_init, FILE_DEVICE_SECURE_OPEN, FALSE);
	WdfDeviceInitAssignName(device_init, &device_name);
	WdfPdoInitAssignRawDevice(device_init, &GUID_DEVCLASS_NET);
	WdfDeviceInitSetDeviceClass(device_init, &GUID_DEVCLASS_NET);

	status = WdfDeviceCreate(&device_init, WDF_NO_OBJECT_ATTRIBUTES, device);

	if (!NT_SUCCESS(status))
	{

		WdfDeviceInitFree(device_init);
		goto $end;
	}

$end:
	return status;

}




NTSTATUS InitializeWfp(PDRIVER_OBJECT driver_obj,PUNICODE_STRING registry_path,WDFDRIVER* driver,WDFDEVICE *device)
{//Wfp초기화함수

	if (!NT_SUCCESS(init_drvier_objects(driver_obj, registry_path, driver, device)))
		goto $end;

	if (!NT_SUCCESS(WfpOpenEngine()))
		goto $end;

	if (!NT_SUCCESS(FwpmTransactionBegin(EngineHandle, 0)))
		goto $end;


	g_device = WdfDeviceWdmGetDeviceObject(device);

	if (!NT_SUCCESS(WfpRegisterCallout()))
		goto $end;


	if (!NT_SUCCESS(WfpAddCallout()))
		goto $end;

	if (!NT_SUCCESS(WfpAddSublayer()))
		goto $end;


	if (!NT_SUCCESS(WfpAddSublayer()))
		goto $end;

	if (!NT_SUCCESS(WfpAddFilter()))
		goto $end;



$end:
	UnInitWfp();
	return STATUS_UNSUCCESSFUL;
}

//먼저 콜 아웃기능을 등록하기전에 드라이버 입력기능
NTSTATUS DriverEntry(PDRIVER_OBJECT driver_obj, PUNICODE_STRING registry_path)
{

	NTSTATUS status;
	WDFDEVICE device = { 0, };
	WDFDRIVER driver = { 0, };
	driver_obj->DriverUnload = Unload;
	

	status = InitializeWfp(driver_obj,registry_path,&driver,&device);

	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(g_device);
	}
	return status;
}

