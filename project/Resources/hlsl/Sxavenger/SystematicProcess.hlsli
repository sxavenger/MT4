#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include "ProcessCommon.hlsli"

//=========================================================================================
// GBuffers
//=========================================================================================

Texture2D<float4> gAlbedo   : register(t10);
Texture2D<float4> gNormal   : register(t11);
Texture2D<float4> gPosition : register(t12);

//=========================================================================================
// ProcessConfig buffer
//=========================================================================================

ConstantBuffer<ProcessConfig> gConfig : register(b10);

////////////////////////////////////////////////////////////////////////////////////////////
// methods
////////////////////////////////////////////////////////////////////////////////////////////

float4 GetAlbedo(uint2 index) {
	return gAlbedo[index];
}

float3 GetNormal(uint2 index) {
	float4 normal = gNormal[index];
	return normalize(normal.rgb * 2.0f - 1.0f);
}

float4 GetPosition(uint2 index) {
	return gPosition[index];
}