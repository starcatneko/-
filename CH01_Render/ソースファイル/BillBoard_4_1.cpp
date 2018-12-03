//------------------------------------------------------------
// BillBoard_4_1.cpp
// ����C�ӂ̕����Ɍ�����
// 
//------------------------------------------------------------

#include <D3D11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include<d3d11shader.h>
#include<DirectXTex\DirectXTex.h>
#include<WICTextureLoader\WICTextureLoader.h>

#define VIEW_WIDTH					800					// ��ʕ�
#define VIEW_HEIGHT					600					// ��ʍ���

#define PI							3.1415927f			// �~����
#define PLAYER_SPEED				0.08f				// �v���C���[�̈ړ����x
#define R							4.0f				// �}�`�̑傫��
#define CORNER_NUM					8					// �p��
#define LINE_NUM					6					// ���C����


// ���_�\����
struct CUSTOMVERTEX {
    XMFLOAT4	v4Pos;
	XMFLOAT2	v2UV;
};


struct MY_PLAYER {
	XMFLOAT3			v3Pos;					// �ʒu
};

MY_PLAYER	Player_1;							// �v���C���[�f�[�^


// �x�N�g���̈����Z
XMFLOAT3 Subtract( XMFLOAT3 *pv3Vec1, XMFLOAT3 *pv3Vec2 )						
{
	return XMFLOAT3( pv3Vec1->x - pv3Vec2->x,
					 pv3Vec1->y - pv3Vec2->y,
					 pv3Vec1->z - pv3Vec2->z );
}


// �x�N�g���̒P�ʉ�
XMFLOAT3 Normalize( XMFLOAT3 *pv3Vec )
{
	float		fLength;

	fLength = sqrtf( pv3Vec->x * pv3Vec->x +
					 pv3Vec->y * pv3Vec->y +
					 pv3Vec->z * pv3Vec->z );

	return XMFLOAT3( pv3Vec->x / fLength, pv3Vec->y / fLength, pv3Vec->z / fLength );
}


// �x�N�g���̊O��
XMFLOAT3 Cross( XMFLOAT3 *pv3Vec1, XMFLOAT3 *pv3Vec2 )
{
	return XMFLOAT3( pv3Vec1->y * pv3Vec2->z - pv3Vec1->z * pv3Vec2->y,
					 pv3Vec1->z * pv3Vec2->x - pv3Vec1->x * pv3Vec2->z,
					 pv3Vec1->x * pv3Vec2->y - pv3Vec1->y * pv3Vec2->x );
}

// ��������s��쐬
// �r���{�[�h�̂悤�Ȕ|���S�������łȂ��A���̕���
// �C�ӂ̕����Ɍ����邱�Ƃ��ł���
// �n�_�E�I�_���󂯎��A���̎n�_����I�_�ւƌ������x�N�g���̕�����
// ����x���̕��������킹��B����z���́A�n�_����I�_�ւƌ������x�N�g����
// �������̃x�N�g��(1,0,0)�̗����ɐ����ȃx�N�g���̕����ɍ��킹��B
// ����y���́A�ϊ��ς�x���ƕϊ��ς�z���̗����ɐ����ȕ����Ɍ�����
XMMATRIX MakeDirectMatrix( XMFLOAT3 *pv3Start, XMFLOAT3 *pv3End )
{
	XMMATRIX		matArrowWorld=XMMatrixIdentity();
	XMFLOAT3		v3Foward, v3Side, v3Up;

	

	return matArrowWorld;
}


int InitPlayer( void )									// �v���C���[�̏�����
{
	// �v���C���[1
	Player_1.v3Pos = XMFLOAT3( 0.0f, 10.0f, 0.0f );

	return 0;
}


int MovePlayer( void )									// ���_�̈ړ�
{
	float fEyeAngle;

	fEyeAngle = 2.0f * PI / 8000.0f * ( timeGetTime() % 8000 );
	Player_1.v3Pos.x = 5.0f * cosf( fEyeAngle );
	Player_1.v3Pos.z = 5.0f * sinf( fEyeAngle );
	if ( GetAsyncKeyState( VK_UP ) ) {
		Player_1.v3Pos.y += PLAYER_SPEED;
	}
	// ��
	if ( GetAsyncKeyState( VK_DOWN ) ) {
		Player_1.v3Pos.y -= PLAYER_SPEED;
	}

	return 0;
}


//------------------------------------------------------------
// �ȉ��ADirectX�ɂ��\���v���O����

#include <stdio.h>
#include <windows.h>
#include <tchar.h>								// Unicode�E�}���`�o�C�g�����֌W


#define MAX_BUFFER_VERTEX				10000	// �ő�o�b�t�@���_��
#define MAX_BUFFER_INDEX				20000	// �ő�o�b�t�@�C���f�b�N�X��
#define MAX_MODEL_NUM					100		// �ő僂�f����


// �����N���C�u����
#pragma comment( lib, "d3d11.lib" )   // D3D11���C�u����
#pragma comment( lib, "d3dcompiler.lib" )
#pragma comment( lib, "DirectXTex.lib" )
#pragma comment( lib, "winmm.lib" )


// �Z�[�t�����[�X�}�N��
#ifndef SAFE_RELEASE
#define SAFE_RELEASE( p )      { if ( p ) { ( p )->Release(); ( p )=NULL; } }
#endif

// �V�F�[�_�萔�\����
struct CBNeverChanges
{
    XMMATRIX mView;
	XMFLOAT4 v4AddColor;
};

// �e�N�X�`���G�\����
struct TEX_PICTURE {
	ID3D11ShaderResourceView	*pSRViewTexture;
	D3D11_TEXTURE2D_DESC		tdDesc;
	int							nWidth, nHeight;
};

// ���f���\����
struct MY_MODEL {
	int					nVertexPos;						// ���_�ʒu
	int					nVertexNum;						// ���_��
	int					nIndexPos;						// �C���f�b�N�X�ʒu
	int					nIndexNum;						// �C���f�b�N�X��
	TEX_PICTURE			*ptpTexture;					// �e�N�X�`��
	XMMATRIX			mMatrix;						// �ϊ��s��
	XMFLOAT4			v4AddColor;						// ���Z�F
	int					bLine;							// ���\��
};


// �O���[�o���ϐ�
UINT  g_nClientWidth;							// �`��̈�̉���
UINT  g_nClientHeight;							// �`��̈�̍���

HWND        g_hWnd;         // �E�B���h�E�n���h��


ID3D11Device			*g_pd3dDevice;			// �f�o�C�X
IDXGISwapChain			*g_pSwapChain;			// DXGI�X���b�v�`�F�C��
ID3D11DeviceContext		*g_pImmediateContext;	// �f�o�C�X�R���e�L�X�g
ID3D11RasterizerState	*g_pRS;					// ���X�^���C�U
ID3D11RasterizerState	*g_pRS_Cull_CW;			// ���X�^���C�U(���v���J�����O)
ID3D11RasterizerState	*g_pRS_Cull_CCW;		// ���X�^���C�U(�����v���J�����O)
ID3D11RenderTargetView	*g_pRTV;				// �����_�����O�^�[�Q�b�g
ID3D11Texture2D*        g_pDepthStencil = NULL;	// Z�o�b�t�@
ID3D11DepthStencilView* g_pDepthStencilView = NULL;	// Z�o�b�t�@�̃r���[
ID3D11DepthStencilState *g_pDSDepthState = NULL;	// Z�o�b�t�@�̃X�e�[�g
ID3D11DepthStencilState *g_pDSDepthState_NoWrite = NULL;	// Z�o�b�t�@�̃X�e�[�g(Z�o�b�t�@�֏������݂Ȃ�)
D3D_FEATURE_LEVEL       g_FeatureLevel;			// �t�B�[�`���[���x��

ID3D11Buffer			*g_pVertexBuffer;		// ���_�o�b�t�@
ID3D11Buffer			*g_pIndexBuffer;		// �C���f�b�N�X�o�b�t�@
ID3D11BlendState		*g_pbsAddBlend;			// ���Z�u�����h
ID3D11VertexShader		*g_pVertexShader;		// ���_�V�F�[�_
ID3D11PixelShader		*g_pPixelShader;		// �s�N�Z���V�F�[�_
ID3D11InputLayout		*g_pInputLayout;		// �V�F�[�_���̓��C�A�E�g
ID3D11SamplerState		*g_pSamplerState;		// �T���v���X�e�[�g

ID3D11Buffer			*g_pCBNeverChanges = NULL;

// �`�撸�_�o�b�t�@
CUSTOMVERTEX g_cvVertices[MAX_BUFFER_VERTEX];
int							g_nVertexNum = 0;

WORD		g_wIndices[MAX_BUFFER_INDEX];
int							g_nIndexNum = 0;

TEX_PICTURE				g_tGroundTexture, g_tBillboardTexture;
//MY_MODEL					g_mmBillboard;
//MY_MODEL					g_mmGround;
MY_MODEL					g_mmSphere;
MY_MODEL					g_mmArrow;
//MY_MODEL					g_mmTriangles[CHECK_TRIANGLE_NUM];


// Direct3D�̏�����
HRESULT InitD3D( void )
{
    HRESULT hr = S_OK;
	D3D_FEATURE_LEVEL  FeatureLevelsRequested[6] = { D3D_FEATURE_LEVEL_11_0,
													 D3D_FEATURE_LEVEL_10_1,
													 D3D_FEATURE_LEVEL_10_0,
													 D3D_FEATURE_LEVEL_9_3,
													 D3D_FEATURE_LEVEL_9_2,
													 D3D_FEATURE_LEVEL_9_1 };
	UINT               numLevelsRequested = 6;
	D3D_FEATURE_LEVEL  FeatureLevelsSupported;

	// �f�o�C�X�쐬
	hr = D3D11CreateDevice( NULL,
					D3D_DRIVER_TYPE_HARDWARE, 
					NULL, 
					0,
					FeatureLevelsRequested, 
					numLevelsRequested,
					D3D11_SDK_VERSION, 
					&g_pd3dDevice,
					&FeatureLevelsSupported,
					&g_pImmediateContext );
	if( FAILED ( hr ) ) {
		return hr;
	}

	// �t�@�N�g���̎擾
	IDXGIDevice * pDXGIDevice;
	hr = g_pd3dDevice->QueryInterface( __uuidof( IDXGIDevice ), ( void ** )&pDXGIDevice );
	IDXGIAdapter * pDXGIAdapter;
	hr = pDXGIDevice->GetParent( __uuidof( IDXGIAdapter ), ( void ** )&pDXGIAdapter );
	IDXGIFactory * pIDXGIFactory;
	pDXGIAdapter->GetParent( __uuidof( IDXGIFactory ), ( void ** )&pIDXGIFactory);

	// �X���b�v�`�F�C���̍쐬
    DXGI_SWAP_CHAIN_DESC	sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = g_nClientWidth;
	sd.BufferDesc.Height = g_nClientHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	hr = pIDXGIFactory->CreateSwapChain( g_pd3dDevice, &sd, &g_pSwapChain );

	pDXGIDevice->Release();
	pDXGIAdapter->Release();
	pIDXGIFactory->Release();

	if( FAILED ( hr ) ) {
		return hr;
	}

    // �����_�����O�^�[�Q�b�g�̐���
    ID3D11Texture2D			*pBackBuffer = NULL;
    D3D11_TEXTURE2D_DESC BackBufferSurfaceDesc;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if ( FAILED( hr ) ) {
		MessageBox( NULL, _T( "Can't get backbuffer." ), _T( "Error" ), MB_OK );
        return hr;
    }
    pBackBuffer->GetDesc( &BackBufferSurfaceDesc );
    hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRTV );
    SAFE_RELEASE( pBackBuffer );
    if ( FAILED( hr ) ) {
		MessageBox( NULL, _T( "Can't create render target view." ), _T( "Error" ), MB_OK );
        return hr;
    }

    // *** Create depth stencil texture ***
    D3D11_TEXTURE2D_DESC descDepth;
	RECT rc;
    GetClientRect( g_hWnd, &rc );
	ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = rc.right - rc.left;
    descDepth.Height = rc.bottom - rc.top;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = g_pd3dDevice->CreateTexture2D( &descDepth, NULL, &g_pDepthStencil );
    if( FAILED( hr ) )
        return hr;

    // *** Create the depth stencil view ***
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
    if( FAILED( hr ) )
        return hr;

	// *** �����_�����O�^�[�Q�b�g�ݒ� ***
    g_pImmediateContext->OMSetRenderTargets( 1, &g_pRTV, g_pDepthStencilView );

	// �X�e���V���X�e�[�g�̍쐬
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	hr = g_pd3dDevice->CreateDepthStencilState( &dsDesc, &g_pDSDepthState );

	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = g_pd3dDevice->CreateDepthStencilState( &dsDesc, &g_pDSDepthState_NoWrite );

//	g_pImmediateContext->OMSetDepthStencilState( g_pDSDepthState, 1 );

    // ���X�^���C�U�̐ݒ�
    D3D11_RASTERIZER_DESC drd;
	ZeroMemory( &drd, sizeof( drd ) );
	drd.FillMode				= D3D11_FILL_SOLID;
	drd.CullMode				= D3D11_CULL_NONE;
	drd.FrontCounterClockwise	= FALSE;
	drd.DepthClipEnable			= TRUE;
    hr = g_pd3dDevice->CreateRasterizerState( &drd, &g_pRS );
    if ( FAILED( hr ) ) {
		MessageBox( NULL, _T( "Can't create rasterizer state." ), _T( "Error" ), MB_OK );
        return hr;
    }
	g_pImmediateContext->RSSetState( g_pRS );

    // ���X�^���C�U�̐ݒ�(���v���J�����O)
	ZeroMemory( &drd, sizeof( drd ) );
	drd.FillMode				= D3D11_FILL_SOLID;
	drd.CullMode				= D3D11_CULL_BACK;
	drd.FrontCounterClockwise	= TRUE;
	drd.DepthClipEnable			= TRUE;
    hr = g_pd3dDevice->CreateRasterizerState( &drd, &g_pRS_Cull_CW );
    if ( FAILED( hr ) ) {
		MessageBox( NULL, _T( "Can't create rasterizer state." ), _T( "Error" ), MB_OK );
        return hr;
    }
//    g_pImmediateContext->RSSetState( g_pRS_Cull_CW );

    // ���X�^���C�U�̐ݒ�(�����v���J�����O)
	ZeroMemory( &drd, sizeof( drd ) );
	drd.FillMode				= D3D11_FILL_SOLID;
	drd.CullMode				= D3D11_CULL_BACK;
	drd.FrontCounterClockwise	= FALSE;
	drd.DepthClipEnable			= TRUE;
    hr = g_pd3dDevice->CreateRasterizerState( &drd, &g_pRS_Cull_CCW );
    if ( FAILED( hr ) ) {
		MessageBox( NULL, _T( "Can't create rasterizer state." ), _T( "Error" ), MB_OK );
        return hr;
    }
//    g_pImmediateContext->RSSetState( g_pRS_Cull_CCW );

    // �r���[�|�[�g�̐ݒ�
    D3D11_VIEWPORT vp;
    vp.Width    = ( FLOAT )g_nClientWidth;
    vp.Height   = ( FLOAT )g_nClientHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    g_pImmediateContext->RSSetViewports( 1, &vp );

    return S_OK;
}


// �v���O���}�u���V�F�[�_�쐬
HRESULT MakeShaders( void )
{
    HRESULT hr;
    ID3DBlob* pVertexShaderBuffer = NULL;
    ID3DBlob* pPixelShaderBuffer = NULL;
    ID3DBlob* pError = NULL;

    DWORD dwShaderFlags = 0;
#ifdef _DEBUG
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
    // �R���p�C��
    hr = D3DX11CompileFromFile( _T( "Basic_3D_TexMark.fx" ), NULL, NULL, "VS", "vs_4_0_level_9_1",
								dwShaderFlags, 0, NULL, &pVertexShaderBuffer, &pError, NULL );
    if ( FAILED( hr ) ) {
		MessageBox( NULL, _T( "Can't open Basic_3D_TexMark.fx" ), _T( "Error" ), MB_OK );
        SAFE_RELEASE( pError );
        return hr;
    }
    hr = D3DX11CompileFromFile( _T( "Basic_3D_TexMark.fx" ), NULL, NULL, "PS", "ps_4_0_level_9_1",
								dwShaderFlags, 0, NULL, &pPixelShaderBuffer, &pError, NULL );
    if ( FAILED( hr ) ) {
        SAFE_RELEASE( pVertexShaderBuffer );
        SAFE_RELEASE( pError );
        return hr;
    }
    SAFE_RELEASE( pError );
    
    // VertexShader�쐬
    hr = g_pd3dDevice->CreateVertexShader( pVertexShaderBuffer->GetBufferPointer(),
										   pVertexShaderBuffer->GetBufferSize(),
										   NULL, &g_pVertexShader );
    if ( FAILED( hr ) ) {
        SAFE_RELEASE( pVertexShaderBuffer );
        SAFE_RELEASE( pPixelShaderBuffer );
        return hr;
    }
    // PixelShader�쐬
    hr = g_pd3dDevice->CreatePixelShader( pPixelShaderBuffer->GetBufferPointer(),
										  pPixelShaderBuffer->GetBufferSize(),
										  NULL, &g_pPixelShader );
    if ( FAILED( hr ) ) {
        SAFE_RELEASE( pVertexShaderBuffer );
        SAFE_RELEASE( pPixelShaderBuffer );
        return hr;
    }

    // ���̓o�b�t�@�̓��͌`��
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
	UINT numElements = ARRAYSIZE( layout );
	// ���̓o�b�t�@�̓��͌`���쐬
    hr = g_pd3dDevice->CreateInputLayout( layout, numElements,
										  pVertexShaderBuffer->GetBufferPointer(),
										  pVertexShaderBuffer->GetBufferSize(),
										  &g_pInputLayout );
    SAFE_RELEASE( pVertexShaderBuffer );
    SAFE_RELEASE( pPixelShaderBuffer );
    if ( FAILED( hr ) ) {
        return hr;
    }

    // �V�F�[�_�萔�o�b�t�@�쐬
    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof( bd ) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( CBNeverChanges );
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pCBNeverChanges );
    if( FAILED( hr ) )
        return hr;

    return S_OK;
}


// �e�N�X�`�����[�h
int LoadTexture( TCHAR *szFileName, TEX_PICTURE *pTexPic, int nWidth, int nHeight,
				 int nTexWidth, int nTexHeight )
{
    HRESULT						hr;
	D3DX11_IMAGE_LOAD_INFO		liLoadInfo;
	ID3D11Texture2D				*pTexture;

	ZeroMemory( &liLoadInfo, sizeof( D3DX11_IMAGE_LOAD_INFO ) );
	liLoadInfo.Width = nTexWidth;
	liLoadInfo.Height = nTexHeight;
	liLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	liLoadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	hr = D3DX11CreateShaderResourceViewFromFile( g_pd3dDevice, szFileName, &liLoadInfo,
												 NULL, &( pTexPic->pSRViewTexture ), NULL );
    if ( FAILED( hr ) ) {
        return hr;
    }
	pTexPic->pSRViewTexture->GetResource( ( ID3D11Resource ** )&( pTexture ) );
	pTexture->GetDesc( &( pTexPic->tdDesc ) );
	pTexture->Release();

	pTexPic->nWidth = nWidth;
	pTexPic->nHeight = nHeight;

	return S_OK;
}


// �`�惂�[�h�I�u�W�F�N�g������
int InitDrawModes( void )
{
    HRESULT				hr;

	// �u�����h�X�e�[�g
    D3D11_BLEND_DESC BlendDesc;
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
    BlendDesc.RenderTarget[0].BlendEnable           = TRUE;
    BlendDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_ONE;
    BlendDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_ONE;
    BlendDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
    BlendDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
    BlendDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
    BlendDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = g_pd3dDevice->CreateBlendState( &BlendDesc, &g_pbsAddBlend );
    if ( FAILED( hr ) ) {
        return hr;
    }

    // �T���v��
    D3D11_SAMPLER_DESC samDesc;
    ZeroMemory( &samDesc, sizeof( samDesc ) );
    samDesc.Filter          = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samDesc.AddressU        = D3D11_TEXTURE_ADDRESS_WRAP;
    samDesc.AddressV        = D3D11_TEXTURE_ADDRESS_WRAP;
    samDesc.AddressW        = D3D11_TEXTURE_ADDRESS_WRAP;
    samDesc.ComparisonFunc  = D3D11_COMPARISON_ALWAYS;
    samDesc.MaxLOD          = D3D11_FLOAT32_MAX;
    hr = g_pd3dDevice->CreateSamplerState( &samDesc, &g_pSamplerState );
    if ( FAILED( hr ) ) {
        return hr;
    }

    return S_OK;
}


// �W�I���g���̏�����
HRESULT InitGeometry( void )
{
	int					i, j;
    HRESULT				hr = S_OK;

    // ���_�o�b�t�@�쐬
    D3D11_BUFFER_DESC BufferDesc;
    BufferDesc.Usage                = D3D11_USAGE_DYNAMIC;
    BufferDesc.ByteWidth            = sizeof( CUSTOMVERTEX ) * MAX_BUFFER_VERTEX;
    BufferDesc.BindFlags            = D3D11_BIND_VERTEX_BUFFER;
    BufferDesc.CPUAccessFlags       = D3D11_CPU_ACCESS_WRITE;
    BufferDesc.MiscFlags            = 0;

    D3D11_SUBRESOURCE_DATA SubResourceData;
    SubResourceData.pSysMem             = g_cvVertices;
    SubResourceData.SysMemPitch         = 0;
    SubResourceData.SysMemSlicePitch    = 0;
    hr = g_pd3dDevice->CreateBuffer( &BufferDesc, &SubResourceData, &g_pVertexBuffer );
    if ( FAILED( hr ) ) {
        return hr;
    }

    // �C���f�b�N�X�o�b�t�@�쐬
    BufferDesc.Usage                = D3D11_USAGE_DYNAMIC;
    BufferDesc.ByteWidth            = sizeof( WORD ) * MAX_BUFFER_INDEX;
    BufferDesc.BindFlags            = D3D11_BIND_INDEX_BUFFER;
    BufferDesc.CPUAccessFlags       = D3D11_CPU_ACCESS_WRITE;
    BufferDesc.MiscFlags            = 0;

	SubResourceData.pSysMem         = g_wIndices;
    hr = g_pd3dDevice->CreateBuffer( &BufferDesc, &SubResourceData, &g_pIndexBuffer );
    if( FAILED( hr ) )
        return hr;

	// �e�N�X�`���쐬
	g_tGroundTexture.pSRViewTexture =  NULL;
	hr = LoadTexture( _T( "Tri_Tex.bmp" ), &g_tGroundTexture, 64, 64, 64, 64 );
    if ( FAILED( hr ) ) {
 		MessageBox( NULL, _T( "Can't open Tri_Tex.bmp" ), _T( "Error" ), MB_OK );
       return hr;
    }
	g_tBillboardTexture.pSRViewTexture =  NULL;
	hr = LoadTexture( _T( "Spot_Tex.bmp" ), &g_tBillboardTexture, 256, 256, 256, 256 );
    if ( FAILED( hr ) ) {
 		MessageBox( NULL, _T( "Can't open Spot_Tex.bmp" ), _T( "Error" ), MB_OK );
       return hr;
    }

	// ���f���쐬
	// �����ɂ�鋅
	// ���_�f�[�^�쐬
	float				fTheta;
	float				fPhi;
	float				fAngleDelta;
	float				fLineAngleDelta;
	int					nIndex;						// �f�[�^�̃C���f�b�N�X
	int					nIndexY;					// x�����C���f�b�N�X
	fAngleDelta = PI / CORNER_NUM;
	fLineAngleDelta = 2.0f * PI / LINE_NUM;
	nIndex = 0;
	fPhi = 0.0f;
	for ( i = 0; i < LINE_NUM; i++ ) {
		fTheta = 0.0f;
		for ( j = 0; j < CORNER_NUM + 1; j++ ) {
			g_cvVertices[g_nVertexNum + nIndex].v4Pos  = XMFLOAT4( R * sinf( fTheta ) * cosf( fPhi ),
																   R * cosf( fTheta ),
																   R * sinf( fTheta ) * sinf( fPhi ), 1.0f );
			g_cvVertices[g_nVertexNum + nIndex].v2UV = XMFLOAT2( fPhi / ( 2.0f * PI ), fTheta / PI );
			nIndex++;
			fTheta += fAngleDelta;
		}
		fPhi += fLineAngleDelta;
	}
	g_mmSphere.nVertexPos = g_nVertexNum;
	g_mmSphere.nVertexNum = nIndex;
	g_nVertexNum += nIndex;

	// �C���f�b�N�X�f�[�^�쐬
	nIndex = 0;
	for ( i = 0; i < LINE_NUM; i++ ) {
		nIndexY = i * ( CORNER_NUM + 1 );
		for ( j = 0; j < CORNER_NUM; j++ ) {
			g_wIndices[g_nIndexNum + nIndex    ] = nIndexY + j;
			g_wIndices[g_nIndexNum + nIndex + 1] = nIndexY + j + 1;
			nIndex += 2;
		}
	}
	g_mmSphere.nIndexPos = g_nIndexNum;
	g_mmSphere.nIndexNum = nIndex;
	g_nIndexNum += nIndex;
	g_mmSphere.ptpTexture = &g_tGroundTexture;
	g_mmSphere.mMatrix = XMMatrixIdentity();
	g_mmSphere.v4AddColor = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
	g_mmSphere.bLine = TRUE;

	// ���
	float				SrcPoints[5][2] = { { 0.00f, 1.0f },
											{ 0.30f, 0.5f },
											{ 0.15f, 0.5f },
											{ 0.15f, 0.0f },
											{ 0.00f, 0.0f } };
	float				fAngle;
	fAngle = 0.0f;
	nIndex = 0;
	for ( i = 0; i <= 6; i++ ) {
		for ( j = 0; j < 5; j++ ) {
			g_cvVertices[g_nVertexNum + nIndex].v4Pos = XMFLOAT4( SrcPoints[j][1], SrcPoints[j][0] * cosf( fAngle ),
																				   SrcPoints[j][0] * sinf( fAngle ), 1.0f );
			g_cvVertices[g_nVertexNum + nIndex].v2UV = XMFLOAT2( i / 4.0f * 10.0f, j / 10.0f * 10.0f );
			nIndex++;
		}
		fAngle += 2.0f * PI / 6;
	}
	g_mmArrow.nVertexPos = g_nVertexNum;
	g_mmArrow.nVertexNum = nIndex;
	g_nVertexNum += nIndex;
	
	int				nIndexZ1, nIndexZ2;
	nIndex = 0;
	for ( i = 0; i < 10; i++ ) {
		nIndexZ1 = i * 5;
		nIndexZ2 = ( i + 1 ) * 5;
		for ( j = 0; j < 4; j++ ) {
			g_wIndices[g_nIndexNum + nIndex    ] = nIndexZ1 + j;
			g_wIndices[g_nIndexNum + nIndex + 1] = nIndexZ2 + j;
			g_wIndices[g_nIndexNum + nIndex + 2] = nIndexZ1 + j + 1;
			g_wIndices[g_nIndexNum + nIndex + 3] = nIndexZ1 + j + 1;
			g_wIndices[g_nIndexNum + nIndex + 4] = nIndexZ2 + j;
			g_wIndices[g_nIndexNum + nIndex + 5] = nIndexZ2 + j + 1;
			nIndex += 6;
		}
	}
	g_mmArrow.nIndexPos = g_nIndexNum;
	g_mmArrow.nIndexNum = nIndex;
	g_nIndexNum += nIndex;
	g_mmArrow.ptpTexture = &g_tGroundTexture;
	g_mmArrow.mMatrix = XMMatrixIdentity();
	g_mmArrow.v4AddColor = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
	g_mmArrow.bLine = FALSE;

	// ���_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�쐬
	D3D11_MAPPED_SUBRESOURCE mappedVertices, mappedIndices;
	hr = g_pImmediateContext->Map( g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertices );
    if( FAILED( hr ) )
        return hr;
	hr = g_pImmediateContext->Map( g_pIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedIndices );
    if( FAILED( hr ) ) {
		g_pImmediateContext->Unmap( g_pVertexBuffer, 0 );
        return hr;
	}
	CopyMemory( mappedVertices.pData,  g_cvVertices, sizeof( CUSTOMVERTEX ) * g_nVertexNum );
	CopyMemory( mappedIndices.pData,  g_wIndices, sizeof( WORD ) * g_nIndexNum );
	g_pImmediateContext->Unmap( g_pVertexBuffer, 0 );
	g_pImmediateContext->Unmap( g_pIndexBuffer, 0 );

	return S_OK;
}


// �I������
int Cleanup( void )
{
    SAFE_RELEASE( g_tGroundTexture.pSRViewTexture );
    SAFE_RELEASE( g_tBillboardTexture.pSRViewTexture );
    SAFE_RELEASE( g_pVertexBuffer );
    SAFE_RELEASE( g_pIndexBuffer );

    SAFE_RELEASE( g_pSamplerState );
    SAFE_RELEASE( g_pbsAddBlend );
    SAFE_RELEASE( g_pInputLayout );
    SAFE_RELEASE( g_pPixelShader );
    SAFE_RELEASE( g_pVertexShader );
    SAFE_RELEASE( g_pCBNeverChanges );

    SAFE_RELEASE( g_pRS );									// ���X�^���C�U
    SAFE_RELEASE( g_pRS_Cull_CW );
    SAFE_RELEASE( g_pRS_Cull_CCW );

	// �X�e�[�^�X���N���A
	if ( g_pImmediateContext ) {
		g_pImmediateContext->ClearState();
		g_pImmediateContext->Flush();
	}

    SAFE_RELEASE( g_pRTV );									// �����_�����O�^�[�Q�b�g
    SAFE_RELEASE( g_pDepthStencil );						// Z�o�b�t�@
    SAFE_RELEASE( g_pDepthStencilView );					// Z�o�b�t�@�̃r���[
    SAFE_RELEASE( g_pDSDepthState );						// Z�o�b�t�@�̃X�e�[�g
    SAFE_RELEASE( g_pDSDepthState_NoWrite );

    // �X���b�v�`�F�[��
    if ( g_pSwapChain != NULL ) {
        g_pSwapChain->SetFullscreenState( FALSE, 0 );
    }
    SAFE_RELEASE( g_pSwapChain );

    SAFE_RELEASE( g_pImmediateContext );					// �f�o�C�X�R���e�L�X�g
    SAFE_RELEASE( g_pd3dDevice );							// �f�o�C�X

	return 0;
}


// ���f���̕`��
int DrawMyModel( MY_MODEL *pmmDrawModel, XMMATRIX *pmViewProjection )
{
    CBNeverChanges	cbNeverChanges;

	if ( pmmDrawModel->bLine ) {
	    g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
	}
	else {
		g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	}

	cbNeverChanges.mView = XMMatrixTranspose( pmmDrawModel->mMatrix * *pmViewProjection );
	cbNeverChanges.v4AddColor = pmmDrawModel->v4AddColor;
	g_pImmediateContext->UpdateSubresource( g_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0 );
	g_pImmediateContext->PSSetShaderResources( 0, 1, &( pmmDrawModel->ptpTexture->pSRViewTexture ) );
	g_pImmediateContext->DrawIndexed( pmmDrawModel->nIndexNum, pmmDrawModel->nIndexPos, pmmDrawModel->nVertexPos );

	return 0;
}


// �E�B���h�E�v���V�[�W��
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}


// �����_�����O
HRESULT Render( void )
{
    // ��ʃN���A
	XMFLOAT4	v4Color = XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f );
    g_pImmediateContext->ClearRenderTargetView( g_pRTV, ( float * )&v4Color );
	// *** Z�o�b�t�@�N���A ***
    g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

    // �T���v���Z�b�g
    g_pImmediateContext->PSSetSamplers( 0, 1, &g_pSamplerState );
    
    // �`��ݒ�
    UINT nStrides = sizeof( CUSTOMVERTEX );
    UINT nOffsets = 0;
    g_pImmediateContext->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &nStrides, &nOffsets );
    g_pImmediateContext->IASetIndexBuffer( g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
    g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    g_pImmediateContext->IASetInputLayout( g_pInputLayout );

    // �V�F�[�_�ݒ�
    g_pImmediateContext->VSSetShader( g_pVertexShader, NULL, 0 );
    g_pImmediateContext->VSSetConstantBuffers( 0, 1, &g_pCBNeverChanges );
    g_pImmediateContext->PSSetShader( g_pPixelShader, NULL, 0 );
    g_pImmediateContext->PSSetConstantBuffers( 0, 1, &g_pCBNeverChanges );
		
	// �ϊ��s��
    CBNeverChanges	cbNeverChanges;
	XMMATRIX		mWorld;
	XMMATRIX		mView;
	XMMATRIX		mProjection;
	XMMATRIX		mViewProjection;

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet( Player_1.v3Pos.x, Player_1.v3Pos.y, Player_1.v3Pos.z, 0.0f );
	XMVECTOR At = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	mView = XMMatrixLookAtLH( Eye, At, Up );

    // Initialize the projection matrix
	mProjection = XMMatrixPerspectiveFovLH( XM_PIDIV4, VIEW_WIDTH / ( FLOAT )VIEW_HEIGHT, 0.01f, 100.0f );

	mViewProjection = mView * mProjection;

    // �`��
	g_pImmediateContext->OMSetDepthStencilState( g_pDSDepthState, 1 );
    g_pImmediateContext->RSSetState( g_pRS_Cull_CW );				// �J�����O����

	// ���ʂɉ��������`��
    g_pImmediateContext->OMSetBlendState( NULL, NULL, 0xFFFFFFFF );
	DrawMyModel( &g_mmSphere, &mViewProjection );
	XMFLOAT4 v4Start, v4End;
	XMFLOAT3 v3Start, v3End;
	int i;
	for ( i = 0; i < g_mmSphere.nIndexNum; i += 2 ) {
		v4Start = g_cvVertices[g_wIndices[g_mmSphere.nIndexPos + i]].v4Pos;
		v4End = g_cvVertices[g_wIndices[g_mmSphere.nIndexPos + i + 1]].v4Pos;
		v3Start = XMFLOAT3( v4Start.x, v4Start.y, v4Start.z );
		v3End = XMFLOAT3( v4End.x, v4End.y, v4End.z );
		g_mmArrow.mMatrix = MakeDirectMatrix( &v3Start, &v3End );
		DrawMyModel( &g_mmArrow, &mViewProjection );
	}

    return S_OK;
}


// �G���g���|�C���g
int WINAPI _tWinMain( HINSTANCE hInst, HINSTANCE, LPTSTR, int )
{
	LARGE_INTEGER			nNowTime, nLastTime;		// ���݂ƂЂƂO�̎���
	LARGE_INTEGER			nTimeFreq;					// ���ԒP��

    // ��ʃT�C�Y
    g_nClientWidth  = VIEW_WIDTH;						// ��
    g_nClientHeight = VIEW_HEIGHT;						// ����
	HRESULT hr = CoInitializeEx(NULL,
		COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	// Register the window class
    WNDCLASSEX wc = { sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
                      _T( "D3D Sample" ), NULL };
    RegisterClassEx( &wc );

	RECT rcRect;
	SetRect( &rcRect, 0, 0, g_nClientWidth, g_nClientHeight );
	AdjustWindowRect( &rcRect, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( _T( "D3D Sample" ), _T( "BillBoard_4_1" ),
						   WS_OVERLAPPEDWINDOW, 100, 20, rcRect.right - rcRect.left, rcRect.bottom - rcRect.top,
						   GetDesktopWindow(), NULL, wc.hInstance, NULL );

    // Initialize Direct3D
    if( SUCCEEDED( InitD3D() ) && SUCCEEDED( MakeShaders() ) )
    {
        // Create the shaders
        if( SUCCEEDED( InitDrawModes() ) )
        {
			if ( SUCCEEDED( InitGeometry() ) ) {					// �W�I���g���쐬
				
				InitPlayer();										// �v���C���[�̏�����
				// Show the window
				ShowWindow( g_hWnd, SW_SHOWDEFAULT );
				UpdateWindow( g_hWnd );
				
				QueryPerformanceFrequency( &nTimeFreq );			// ���ԒP��
				QueryPerformanceCounter( &nLastTime );				// 1�t���[���O����������

				// Enter the message loop
				MSG msg;
				ZeroMemory( &msg, sizeof( msg ) );
				while( msg.message != WM_QUIT )
				{
					MovePlayer();
					Render();
					do {
						if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
						{
							TranslateMessage( &msg );
							DispatchMessage( &msg );
						}
						QueryPerformanceCounter( &nNowTime );
					} while( ( ( nNowTime.QuadPart - nLastTime.QuadPart ) < ( nTimeFreq.QuadPart / 90 ) ) &&
							 ( msg.message != WM_QUIT ) );
					while( ( ( nNowTime.QuadPart - nLastTime.QuadPart ) < ( nTimeFreq.QuadPart / 60 ) ) &&
						   ( msg.message != WM_QUIT ) )
					{
						QueryPerformanceCounter( &nNowTime );
					}
					nLastTime = nNowTime;
					g_pSwapChain->Present( 0, 0 );					// �\��
				}
			}
        }
    }

    // Clean up everything and exit the app
    Cleanup();
    UnregisterClass( _T( "D3D Sample" ), wc.hInstance );
    return 0;
}

