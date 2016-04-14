//********************************************************************
//																									
//		３Ｄ用シェーダー		
//
//********************************************************************
//------------------------------------------------------
//		環境関連
//------------------------------------------------------
float4x4 Projection;	//	投影変換行列
float4x4 TransMatrix;	//　ワールド変換
float4x4 matView;		//　ビュー変換
						
//------------------------------------------------------
//		テクスチャサンプラー	
//------------------------------------------------------
//WRAP...反対側から同じ絵を繰り返します
//CLAMP...端の色を繰り返します
//BORDER...透明色にします
texture Texture;
sampler DecaleSamp = sampler_state
{
	Texture = <Texture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};
texture NormalMap;	//	法線マップテクスチャ
sampler NormalSamp = sampler_state
{
	Texture = <NormalMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

texture SpecularMap;	//	スペキュラマップテクスチャ
sampler SpecularSamp = sampler_state
{
	Texture = <SpecularMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

texture HeightMap;		//	高さマップテクスチャ
sampler HeightSamp = sampler_state
{
	Texture = <HeightMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};


//------------------------------------------------------
//		頂点フォーマット
//------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;

	float4 wPos			: TEXCOORD1;//　ピクセルに送る情報にワールドからのポジション追加
	float4 ProjectionPos: TEXCOORD2;//　ピクセルに送る情報にゲーム画面ラストのポジション追加
};

struct VS_INPUT
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float4 Color  : COLOR0;
	float2 Tex	  : TEXCOORD0;
};

//********************************************************************
//
//		基本３Ｄシェーダー		
//
//********************************************************************
//------------------------------------------------------
//		頂点シェーダー	
//------------------------------------------------------
VS_OUTPUT VS_Basic(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	//TransMatrixとPosを合成してwPosの情報生成
	//float4 wpos;
	//wpos =  mul(In.Pos, TransMatrix);
	Out.wPos = mul(In.Pos, TransMatrix);

	Out.Pos = mul(In.Pos, Projection);
	Out.Tex = In.Tex;
	Out.Color = 1.0f;

	Out.ProjectionPos = Out.Pos;

	return Out;
}
//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_Basic(VS_OUTPUT In) : COLOR
{
	float4	OUT;

	//	ピクセル色決定
	OUT = In.Color * tex2D(DecaleSamp, In.Tex);

	//if (In.wPos.y >= 20) {
	//	OUT.r = 1.0;
	//}
	

return OUT;
}

//------------------------------------------------------
//		通常描画テクニック
//------------------------------------------------------
technique copy
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;

		VertexShader = compile vs_2_0 VS_Basic();
		PixelShader = compile ps_2_0 PS_Basic();
	}
}

//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_Shed(VS_OUTPUT In) : COLOR
{
	float4	OUT;

//	ピクセル色決定
OUT = In.Color * tex2D(DecaleSamp, In.Tex);

OUT.rgb -= 0.5f;


return OUT;
}

//------------------------------------------------------
//		通常描画テクニック
//------------------------------------------------------
technique shed
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;

		VertexShader = compile vs_2_0 VS_Basic();
		PixelShader = compile ps_2_0 PS_Shed();
	}
}


//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_Red(VS_OUTPUT In) : COLOR
{
	float4	OUT;

	//	ピクセル色決定
	OUT = In.Color * tex2D(DecaleSamp, In.Tex);

	OUT.r += 0.5f;


	return OUT;
}

//------------------------------------------------------
//		通常描画テクニック
//------------------------------------------------------
technique red
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;

		VertexShader = compile vs_2_0 VS_Basic();
		PixelShader = compile ps_2_0 PS_Red();
	}
}


//**************************************
//
///		ＵＶアニメーション
//
//**************************************

// UV用
sampler DecaleSampUV = sampler_state
{
	Texture = <Texture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	//BorderColor = 0x00FFFFFF;
	AddressU = WRAP;
	AddressV = WRAP;
};


struct VS_INPUT_UV
{
	float4 Pos    : POSITION;
	float3 Normal : NORMAL;
	float2 Tex	  : TEXCOORD0;
	float4 Color	: COLOR0;
};
struct VS_OUTPUT_UV
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;
};

// UV
float tuAnime = 0.0f;
float tvAnime = 0.0f;
float alphaUV = 1.0f;

VS_OUTPUT_UV VS_UvAnime(VS_INPUT_UV In)
{
	VS_OUTPUT_UV Out = (VS_OUTPUT_UV)0;

	Out.Pos = mul(In.Pos, Projection);
	Out.Color = In.Color;// 頂点カラー取得
	Out.Tex = In.Tex + float2(tuAnime, tvAnime);//座標

	Out.Color.rgb = 1.0f;
	Out.Color.a *= alphaUV; //　透明度

	return Out;
}


//------------------------------------------------------
///		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_UvAnime(VS_OUTPUT_UV In) : COLOR
{
	float4	OUT;
//	ピクセル色決定
OUT = In.Color * tex2D(DecaleSampUV, In.Tex);
return OUT;
}

technique uvAnime
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;			// このオブジェクトはZバッファを考慮する
		ZWriteEnable = false;	// このオブジェクトをZバッファに書き込まない

		VertexShader = compile vs_3_0 VS_UvAnime();
		PixelShader = compile ps_3_0 PS_UvAnime();
	}
}

// 乗算用
technique uvAnime_add
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = one;
		CullMode = CCW;
		ZEnable = true;			// このオブジェクトはZバッファを考慮する
		ZWriteEnable = false;	// このオブジェクトをZバッファに書き込まない

		VertexShader = compile vs_3_0 VS_UvAnime();
		PixelShader = compile ps_3_0 PS_UvAnime();
	}

}

