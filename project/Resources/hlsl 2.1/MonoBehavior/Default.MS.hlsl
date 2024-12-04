//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include "DefaultMesh.hlsli"

////////////////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////////////////
[numthreads(128, 1, 1)]
[outputtopology("triangle")]
void main(
	uint groupThreadId : SV_GroupThreadID,
	uint groupId : SV_GroupID,
	in payload Payload payload,
	out vertices PSInput  verts[_MAX_VERTICES],
	out indices uint3     polys[_MAX_INDICES]) {
	
	uint meshletIndex = payload.meshletIndices[groupId];
	
	if (meshletIndex >= gMeshInfo.meshletCount) {
		return;
	}
	
	Meshlet m = gMeshlets[meshletIndex];
	
	SetMeshOutputCounts(m.vertexCount, m.primitiveCount);
	
	if (groupThreadId < m.primitiveCount) {
		uint packedIndex = gPrimitives[m.primitiveOffset + groupThreadId];
		polys[groupThreadId] = UnpackPrimitiveIndex(packedIndex);
	}
	
	if (groupThreadId < m.vertexCount) {
		uint index        = gIndices[m.vertexOffset + groupThreadId];
		VertexInput input = gVertices[index];
		
		PSInput output;
		
		output.position = mul(input.position, mul(gTransform.world, viewProj));
		output.worldPos = mul(input.position, gTransform.world).xyz;
		output.texcoord = input.texcoord;
		output.normal   = normalize(mul(input.normal, (float3x3)gTransform.world));

		verts[groupThreadId] = output;
	}
}