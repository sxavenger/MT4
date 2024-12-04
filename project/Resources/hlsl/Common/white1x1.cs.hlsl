//=========================================================================================
// output buffer
//=========================================================================================

RWTexture2D<float4> gOutput : register(u0);

////////////////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////////////////
[numthreads(1, 1, 1)]
void main(uint3 groupThreadId : SV_GroupThreadID) {

	gOutput[groupThreadId.xy] = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
}