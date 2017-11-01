texture g_TexBase;
texture g_TexLayer1;
texture g_TexLayer2;
texture g_TexLayer3;
texture g_TexLayer4;
texture g_TexAlpha;

float4x4 g_mWorldViewProjection;    // World * View * Projection matrix

sampler TexBaseSampler = sampler_state { Texture = < g_TexBase >; MipFilter = LINEAR; MinFilter = LINEAR; MagFilter = LINEAR; };
sampler TexLayer1Sampler = sampler_state { Texture = < g_TexLayer1 >; MipFilter = LINEAR; MinFilter = LINEAR; MagFilter = LINEAR; };
sampler TexLayer2Sampler = sampler_state { Texture = < g_TexLayer2 >; MipFilter = LINEAR; MinFilter = LINEAR; MagFilter = LINEAR; };
sampler TexLayer3Sampler = sampler_state { Texture = < g_TexLayer3 >; MipFilter = LINEAR; MinFilter = LINEAR; MagFilter = LINEAR; };
sampler TexLayer4Sampler = sampler_state { Texture = < g_TexLayer4 >; MipFilter = LINEAR; MinFilter = LINEAR; MagFilter = LINEAR; };
sampler TexAlphaSampler = sampler_state { Texture = < g_TexAlpha >; MipFilter = LINEAR; MinFilter = LINEAR; MagFilter = LINEAR; };

struct VS_OUTPUT
{
	float4 position : POSITION;
	float4 normal	: NORMAL;
	float2 texCoord0 : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
};

VS_OUTPUT RenderSceneVS(float4 inPosition : POSITION,
	float4 inNormal : NORMAL,
	float2 inTexCoord0 : TEXCOORD0,
	float2 inTexCoord1 : TEXCOORD1)
{
	VS_OUTPUT Out;

	Out.position = mul(inPosition, g_mWorldViewProjection);
	Out.normal = mul(inNormal, g_mWorldViewProjection);
	inNormal = normalize(inNormal);
		
	Out.texCoord0 = inTexCoord0;
	Out.texCoord1 = inTexCoord1;	

	return Out;
}

struct PS_OUTPUT
{
	float4 RGBColor : COLOR0;
};

float2 RotateUv(float2 in_uv, float degree) {

	float2 out_uv;
	float rad = radians(degree);
	out_uv.x = in_uv.x * cos(rad) - in_uv.y * sin(rad);
	out_uv.y = in_uv.y * cos(rad) + in_uv.x * sin(rad);
	out_uv = out_uv + 1.0;
	return out_uv;
}

float4 RenderScenePS(	float2 inTexCoord0 : TEXCOORD0,
						float2 inTexCoord1 : TEXCOORD1) : COLOR0
{
	float4 bcolor = tex2D(TexBaseSampler, inTexCoord0);
	float4 l1color = tex2D(TexLayer1Sampler, inTexCoord0);	
	float4 l2color = tex2D(TexLayer2Sampler, inTexCoord0);
	float4 l3color = tex2D(TexLayer3Sampler, inTexCoord0);
	float4 l4color = tex2D(TexLayer4Sampler, inTexCoord0);

	float4 alpha1 = tex2D(TexAlphaSampler, inTexCoord1);	
	float4 alpha2 = tex2D(TexAlphaSampler, RotateUv(inTexCoord1, 90));
	float4 alpha3 = tex2D(TexAlphaSampler, RotateUv(inTexCoord1, 180));
	float4 alpha4 = tex2D(TexAlphaSampler, RotateUv(inTexCoord1, 270));
	
	float4 resultColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	
	resultColor = alpha1 * bcolor + (1.0f - alpha1) * l1color;
	resultColor = alpha2 * resultColor + (1.0f - alpha2) * l2color;
	resultColor = alpha3 * resultColor + (1.0f - alpha3) * l3color;
	resultColor = alpha4 * resultColor + (1.0f - alpha4) * l4color;

	resultColor = saturate(resultColor);

	return resultColor;
}

technique RenderTerrain
{
	pass P0
	{
		VertexShader = compile vs_3_0 RenderSceneVS();
		PixelShader = compile ps_3_0 RenderScenePS();
	}
}