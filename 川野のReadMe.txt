�����̐��w�T���v����2010�N�����
DX11(OS��Win7�����������DirectX2010June)��z�肵�č���Ă��܂��B
���̂��߁A���̂܂܂ŃR���p�C�����ē��������Ƃ͏o���܂���B

�F�X�ƃv���O�����I�Ȏ�Ԃ��K�v�ɂȂ�܂��B

����������Ă鎞�_�ŁA��̏��Ђ̃\�[�X�R�[�h�͍X�V����Ă��Ȃ��݂����ł���
���O�ŉ��Ƃ����邵�������ł��B
�z���}�́A�F����Q�[���v���O���}�R�N���Ȃ�ł�����H
�����̗݂͂̂ŉ��Ƃ����ė~�����Ƃ���ł����A�܂��唼�������ł��傤���B

�ǂ������ύX���Ƃ�����D3DX�n�����s���g�p�s�ƂȂ��Ă���A����̑�֕���
�T���āA������g�p����悤�Ƀv���O����������������K�v���o�Ă��܂��B

CH02_Polygons�܂ł�
�@�C���N���[�h���̕ύX
�A�����N���郉�C�u�����̕ύX
�Busing namespace DirectX;�̒ǉ�
�CCreateShaderFromFile�̕ύX
�D�s��̎d�l���ύX����Ă���̂ŁA���̕ύX

�łȂ�Ƃ��Ȃ�܂��B
�@�C���N���[�h���̕ύX
#include <D3D11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>

�A�����N���郉�C�u�����̕ύX
#pragma comment( lib, "d3d11.lib" )   // D3D11���C�u����
#pragma comment( lib, "d3dcompiler.lib" )

�Busing namespace DirectX;�̒ǉ�


�CCreateShaderFromFile�̕ύX
// �R���p�C��
hr = D3DCompileFromFile( _T( "Basic_2D_Geom.fx" ), nullptr, nullptr, "VS", "vs_4_0_level_9_1",
							dwShaderFlags, 0,  &pVertexShaderBuffer, &pError );
if ( FAILED( hr ) ) {
	MessageBox( NULL, _T( "Can't open Basic_2D_Geom.fx" ), _T( "Error" ), MB_OK );
    SAFE_RELEASE( pError );
    return hr;
}
hr = D3DCompileFromFile( _T( "Basic_2D_Geom.fx" ), nullptr, nullptr, "PS", "ps_4_0_level_9_1",
							dwShaderFlags, 0,  &pPixelShaderBuffer, &pError );


�D�s��̎d�l���ύX����Ă���̂ŁA���̕ύX
mScreen = XMMatrixIdentity();
mScreen.r[0].m128_f32[0] =  2.0f / g_nClientWidth;
mScreen.r[1].m128_f32[1] = -2.0f / g_nClientHeight;
mScreen.r[3].m128_f32[0] = -1.0f;
mScreen.r[3].m128_f32[1] =  1.0f;


�ōς�ł܂����B�l�ɂ���Ă͂���ł���ςł��傤���A���̒��x�ōςނ����}�V�ł��B

�Ƃ��낪CH03_Textures����͂���ɖʓ|�Ȃ��ƂɂȂ�܂��B
�e�N�X�`�����[�h�֘A���Ȃ��Ȃ��Ă�̂ŁA��{�I�ɂ͎��O�ł��Ȃ���΂Ȃ�܂���B
�Ƃ͂����A�g�b�v���x���Ȃ�e���p�A�S���ɂ͖����ł��傤( �O�́O)ɼ

�Ƃ������ƂŁADirectXTex�Ƃ������C�u�������g�p���܂��B
https://github.com/Microsoft/DirectXTex
Microsoft�̃��C�u�����Ȃ̂ŁA�܂��M�p���Ă����ł��傤�B
Clone Or Download���N���b�N���ă_�E�����[�h���Ă��������B

�ŁADirectXTex_Desktop_2015_Win10.sln���R���p�C���B�o���オ�������C�u�����ւ̃p�X��ʂ��ă����N
�C���N���[�h�t�@�C���Ƃ���DirectXTex�̒��ɂ���h�t�@�C����inl�t�@�C���ւ̃p�X��ʂ��Ă����܂��B

�����A���ꂾ���ł�png��bmp�̃��[�h���ł��܂���B���̂���WICTextureLoader���g�p���܂��B
WICTextureLoader�t�H���_�̒��ɍs���āAWICTextureLoader.h���C���N���[�h���A�܂��Acpp�t�@�C���̕���
�v���W�F�N�g�ɒǉ����܂��B


�����Ă���ŏI���Ȃ��B
�e�N�X�`�����[�h�����Ƀo�O���������Ă��邽�߁A
hr=CreateWICTextureFromFile(g_pd3dDevice,g_pImmediateContext , szFileName, &resource, &(pTexPic->pSRViewTexture));

�ɏ��������܂��B
�����A����ł����s���G���[���������܂��B���͂����̃��C�u������COM����Ղɂ����Ă��邽��
ComInitialize���Ȃ���΂Ȃ�܂���B

WinMain�ɂ�CreateWindow�̒��キ�炢��
HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
�ƋL�q���Ă��������B

���[�[�[�[�I�I�I�I
�����������������������I�I�I�߂�ǂ������[�[�[�[�[�I�I�I
�Ƃ肠���������܂ł��Ε\�������邱�Ƃ��ł��܂��B�߂�ǂ��������ǕK�{�Ȃ̂ł��肢���܂��B
