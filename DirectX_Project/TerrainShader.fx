texture g_Texture1;
texture g_Texture2;
texture g_Texture3;

texture g_AlphaSide;
texture g_AlphaCorner;
texture g_AlphaFull;
texture g_AlphaDiag;

unsigned int g_TexBackIndex;
unsigned int g_TexFrontIndex;
unsigned int g_AlghaIndex;
unsigned int g_AlghaRotationIndex;

float4x4 g_mWorldViewProjection;    // World * View * Projection matrix

sampler TextureSampler1 = sampler_state
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
sampler TextureSampler3 =
sampler_state
{
	Texture = < g_Texture3 >;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler AlphaSideSampler =
sampler_state
{
	Texture = < g_AlphaSide >;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};
sampler AlphaCornerSampler =
sampler_state
{
	Texture = < g_AlphaCorner >;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

sampler AlphaFullSampler =
sampler_state
{
	Texture = < g_AlphaFull >;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

sampler AlphaDiagSampler =
sampler_state
{
	Texture = < g_AlphaDiag >;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

float2 RotateUv(float2 in_uv, float degree) {
	
	float2 out_uv;
	
	float rad = radians(degree);

	out_uv.x = in_uv.x * cos(rad) - in_uv.y * sin(rad);
	out_uv.y = in_uv.y * cos(rad) + in_uv.x * sin(rad);
	
	out_uv = out_uv + 1.0;

	return out_uv;
}

struct VS_OUTPUT
{
	float4 position : POSITION;
	float4 normal	: NORMAL;
	float2 texCoord0 : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
};

VS_OUTPUT RenderSceneVS(float4 inPosition : POSITION,
						float4 inNormal : NORMAL,
						float2 inTexCoord0 : TEXCOORD0,
						float2 inTexCoord1 : TEXCOORD1,
						float2 inTexCoord2 : TEXCOORD2)
{	
	VS_OUTPUT Out;
	
	Out.position = mul(inPosition, g_mWorldViewProjection);
	Out.normal = mul(inNormal, g_mWorldViewProjection);
	inNormal = normalize(inNormal);

	float2 alphaUV = inTexCoord2;

	[flatten] switch (g_AlghaRotationIndex) {
	case (0):
		break;
	case (1):
		alphaUV = RotateUv(inTexCoord2, 270);
		break;
	case (2):
		alphaUV = RotateUv(inTexCoord2, 180);		
		break;
	case (3):
		alphaUV = RotateUv(inTexCoord2, 90);		
		break;
	default:
		break;
	}

	Out.texCoord0 = inTexCoord0;
	Out.texCoord1 = inTexCoord1;
	Out.texCoord2 = alphaUV;

	return Out;
}

struct PS_OUTPUT
{
	float4 RGBColor : COLOR0;
};
/*
struct PS_INPUT
{	
	float2 texCoord0 : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
};
*/
float4 RenderScenePS(	float2 inTexCoord0 : TEXCOORD0,
						float2 inTexCoord1 : TEXCOORD1,
						float2 inTexCoord2 : TEXCOORD2) : COLOR0
{
	float4 color1 = tex2D(TextureSampler1, inTexCoord0);
	float4 color2 = tex2D(TextureSampler2, inTexCoord1);
	float4 alpha = tex2D(AlphaCornerSampler, inTexCoord2);

	[flatten] switch (g_TexBackIndex) {
		case (0):
			color1 = tex2D(TextureSampler1, inTexCoord0);
			break;
		case (1):
			color1 = tex2D(TextureSampler2, inTexCoord0);
			break;
		case (2):
			color1 = tex2D(TextureSampler3, inTexCoord0);
			break;
		default:
			break;
	}	

	[flatten] switch (g_TexFrontIndex) {
		case (0):
			color2 = tex2D(TextureSampler1, inTexCoord1);
			break;
		case (1):
			color2 = tex2D(TextureSampler2, inTexCoord1);
			break;
		case (2):
			color2 = tex2D(TextureSampler3, inTexCoord1);
			break;
		default:
			break;
	}
	
	[flatten] switch (g_AlghaIndex) {
		case (0):
			alpha = tex2D(AlphaSideSampler, inTexCoord2);
			break;
		case (1):
			alpha = tex2D(AlphaCornerSampler, inTexCoord2);
			break;		
		case (2):
			alpha = tex2D(AlphaFullSampler, inTexCoord2);
			break;
		case (3):
			alpha = tex2D(AlphaDiagSampler, inTexCoord2);
			break;
		default:
			break;
	}
		
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