texture g_Texture;
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix

sampler TextureSampler =
sampler_state
{
	Texture = < g_Texture >;
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
	float4 cResultColor = tex2D(TextureSampler, i.texCoord);
	return cResultColor;
}

technique RenderTerrain
{
	pass P0
	{
		VertexShader = compile vs_3_0 RenderSceneVS();
		PixelShader = compile ps_3_0 RenderScenePS();
	}
}