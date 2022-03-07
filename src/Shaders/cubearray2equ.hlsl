TextureCube<float4> input_cube_map : register(t0);

RWTexture2DArray<float4> output_texture : register(u0);

SamplerState cube_sampler : register(s0);

#define PI 3.141592653589793238463

[numthreads(1, 1, 1)]
void ConverterMain(uint3 id : SV_DispatchThreadID)
{
	uint w, h, e;
	output_texture.GetDimensions(w, h, e);

	float2 uv = (id.xy / float2(w, h)) * 2 - 1;

	float theta = uv.x * PI;
	float phi = (uv.y * PI) / 2.0;

	float x = cos(phi) * sin(theta);
	float y = sin(phi);
	float z = cos(phi) * cos(theta);

	output_texture[id] = input_cube_map.SampleLevel(cube_sampler, float3(z, x, -y), 0);
}