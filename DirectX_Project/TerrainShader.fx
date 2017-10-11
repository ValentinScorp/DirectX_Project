texture g_Texture1;
texture g_Texture2;
texture g_Alpha;

float4x4 g_mWorldViewProjection;    // World * View * Projection matrix

sampler TextureSampler1 =
sampler_state
{
	Texture = < g_Texture1 >;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler TextureSampler2 =
sampler_state
{
	Texture = < g_Texture2 >;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler AlphaSampler =
sampler_state
{
	Texture = < g_Alpha >;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

struct VS_OUTPUT
{
	float4 position          : POSITION;
	float2 texCoord          : TEXCOORD0;
};

VS_OUTPUT RenderSceneVS(float4 inPosition : POSITION,
						float2 inTexCoord : TEXCOORD0)
{	
	VS_OUTPUT Out;
	//inPosition.w = 1;
	// Transform and output input position 
	Out.position = mul(inPosition, g_mWorldViewProjection);

	// Propagate texture coordinate through:	
	Out.texCoord = inTexCoord;

	return Out;
}

struct PS_OUTPUT
{
	float4 RGBColor : COLOR0;
};

struct PS_INPUT
{	
	float2 texCoord : TEXCOORD0;
};

float4 RenderScenePS(PS_INPUT i) : COLOR0
{	
	float4 color1 = tex2D(TextureSampler1, i.texCoord);
	float4 color2 = tex2D(TextureSampler2, i.texCoord);
	float4 alpha = tex2D(AlphaSampler, i.texCoord);
	
	float4 alphaColor1 = color1 * alpha;
	float4 alphaColor2 = color2 * (1.0 - alpha);

	float4 resultColor = alphaColor1 + alphaColor2;
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