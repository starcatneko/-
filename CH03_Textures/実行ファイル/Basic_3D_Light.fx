// ---------------------------------------------------------
// Basic_3D_Light.fx
// 3D���������t���V�F�[�_
// ---------------------------------------------------------

// �萔�o�b�t�@
cbuffer cbNeverChanges : register( b0 )
{
    matrix View;
    matrix World;
	float3 v3Light;
	float  Ambient;
	float  Directional;
};


// VertexShader���͌`��
struct VS_INPUT {
    float4 v4Position	: POSITION;		// �ʒu
    float3 v3Normal		: NORMAL;		// �@���x�N�g��
};

// VertexShader�o�͌`��
struct VS_OUTPUT {
    float4 v4Position	: SV_POSITION;	// �ʒu
    float4 v4Color		: COLOR;		// �F
};

// ���_�V�F�[�_
VS_OUTPUT VS( VS_INPUT Input )
{
    VS_OUTPUT	Output;
	float3		v3WorldNormal;
	float		fPlusDot;
	float		fBright;

    Output.v4Position = mul( Input.v4Position, View );
	v3WorldNormal = mul( Input.v3Normal, World );
	fPlusDot = max( dot( v3WorldNormal, v3Light*2 ), 0.0 );
    fBright = Directional * fPlusDot + Ambient;
    Output.v4Color = float4( fBright, 128,0, 1 );

    return Output;
}

// �s�N�Z���V�F�[�_
float4 PS( VS_OUTPUT Input ) : SV_TARGET {
    return Input.v4Color;
}
