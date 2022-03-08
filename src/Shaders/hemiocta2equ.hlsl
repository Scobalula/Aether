Texture2D<float4> input_texture : register(t0);

RWTexture2D<float4> output_texture : register(u0);

SamplerState input_sampler : register(s0);

#define PI 3.141592653589793238463

float3 DirectionToOctahdeonUV(float3 dir) 
{
	float3 final;
	dir /= abs(dir.x) + abs(dir.y) + abs(dir.z);

	if(dir.y >= 0)
	{
		final.x = dir.x - dir.z;
		final.y = dir.x + dir.z;
		final.z = true;
	}
	else 
	{
		final.x = dir.z - dir.x;
		final.y = dir.z + dir.x;
		final.z = false;
	}

	final.xy = final.xy * 0.5 + 0.5;

	return final;
}

[numthreads(1, 1, 1)]
void ConverterMain(uint3 id : SV_DispatchThreadID)
{
	uint w, h;
	output_texture.GetDimensions(w, h);

	float2 uv = (id.xy / float2(w, h)) * 2 - 1;

	float theta = uv.x * PI;
	float phi = (uv.y * PI) / 2.0;

	float x = cos(phi) * sin(theta);
	float y = sin(phi);
	float z = cos(phi) * cos(theta);

	float3 final = DirectionToOctahdeonUV(float3(-x, -y, z));

    final.x *= 0.5;

	// Lower hemisphere (TODO: just make it apart of if insize Dir to Octa UV)
	if(!final.z)
	{
		final.x += 0.5;
	}

	output_texture[id.xy] = input_texture.SampleLevel(input_sampler, final.xy, 0);
}