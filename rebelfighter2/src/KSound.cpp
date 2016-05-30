#include "_StdAfx.h"
void   KSound::Volume(int iIndex, float fVolume, bool bUp)
{
	float fCurrentVolume;
	m_pChannel[iIndex]->getVolume(&fCurrentVolume);
	if (bUp) fCurrentVolume += fVolume;
	else      fCurrentVolume -= fVolume;
	m_pChannel[iIndex]->setVolume(fCurrentVolume);

}
bool KSound::Init()
{

	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(g_iMaxSound, FMOD_INIT_NORMAL, 0);

	return true;
};
int   KSound::Load(char* pLoadName)
{
	m_pSystem->createSound(pLoadName,
		FMOD_HARDWARE, 0, &m_pSound[m_iSoundList++]);

	//m_pSound[m_iSoundList - 1]->setMode(FMOD_LOOP_OFF);

	return m_iSoundList - 1;
}
int   KSound::Load(char* pLoadName, bool bLoop)
{
	m_pSystem->createSound(pLoadName,
		FMOD_HARDWARE, 0, &m_pSound[m_iSoundList++]);

	if(bLoop == true)
		m_pSound[m_iSoundList-1]->setMode(FMOD_LOOP_NORMAL);

	return m_iSoundList - 1;
}
void  KSound::Play(int iIndex, bool bPlay)
{
	bool playing = false;
	
	if (bPlay == false)
	{
		if (m_pChannel[iIndex] != NULL)
		{
			m_pChannel[iIndex]->isPlaying(&playing);
		}
	}
	if (playing == false)
	{		
		m_pSystem->playSound(FMOD_CHANNEL_FREE,
			m_pSound[iIndex], false, &m_pChannel[iIndex]);
			//m_pSound[iIndex], false, &m_pChannela);
	}
	
}

void	KSound::Frame() {
	m_pSystem->update();
}
void  KSound::Release()
{
	for (int iSound = 0; iSound < g_iMaxSound; iSound++)
	{
		m_pSound[iSound]->release();
	}
	m_pSystem->close();
	m_pSystem->release();
}
KSound::KSound()
{
	m_iSoundList = 0;
}


KSound::~KSound()
{
}
