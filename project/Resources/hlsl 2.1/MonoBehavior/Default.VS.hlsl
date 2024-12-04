//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include "MonoBehavior.hlsli"

//=========================================================================================
// Buffer
//=========================================================================================
ConstantBuffer<TransformationMatrix> gTransform : register(b0);

////////////////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////////////////
PSInput main(VertexInput input) {
	
	PSInput output;
	
	output.position = mul(input.position, mul(gTransform.world, viewProj));
	output.texcoord = input.texcoord;
	output.normal   = normalize(mul(input.normal, (float3x3)gTransform.worldInverseTranspose));
	output.worldPos = mul(input.position, gTransform.world).xyz;
	
	return output;
	
}