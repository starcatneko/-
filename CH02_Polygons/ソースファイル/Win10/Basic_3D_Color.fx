// ---------------------------------------------------------
// Basic_3D_Color.fx
// Simple3D�V�F�[�_(���_�ɐF�t��)
// ---------------------------------------------------------


// �萔�o�b�t�@
cbuffer cbNeverChanges : register( b0 )
{
    matrix View;
};


// VertexShader���͌`��
struct VS_INPUT {
    float4 v4Position	: POSITION;		// �ʒu
    float4 v4Color		: COLOR;		// �F
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

    Output.v4Position = mul( Input.v4Position, View );
    Output.v4Color = Input.v4Color;

    return Output;
}

// �s�N�Z���V�F�[�_
float4 PS( VS_OUTPUT Input ) : SV_TARGET {
    return Input.v4Color;
}
