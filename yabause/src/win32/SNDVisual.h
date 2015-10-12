#define SNDCORE_VISUAL 100


static int SNDVisualInit(void)
{
   return 0;
}

static void SNDVisualDeInit(void)
{
}

static int SNDVisualReset(void)
{
   return 0;
}

static int SNDVisualChangeVideoFormat(UNUSED int vertfreq)
{
   return 0;
}

static void SNDVisualUpdateAudio(UNUSED u32 *leftchanbuffer, UNUSED u32 *rightchanbuffer, UNUSED u32 num_samples)
{
	store_audio(leftchanbuffer,rightchanbuffer,num_samples);
}

static u32 SNDVisualGetAudioSpace(void)
{
   /* A "hack" to get Visual sound core working enough
    * so videos are not "freezing". Values have been
    * found by experiments... I don't have a clue why
    * they are working ^^;
    */
   static int i = 0;
   i++;
   if (i == 55) {
      i = 0;
      return 85;
   } else {
      return 0;
   }
}

void SNDVisualMuteAudio(void)
{
}

void SNDVisualUnMuteAudio(void)
{
}

void SNDVisualSetVolume(int volume)
{
}

SoundInterface_struct SNDVisual = {
SNDCORE_VISUAL,
"Visual Sound Interface",
SNDVisualInit,
SNDVisualDeInit,
SNDVisualReset,
SNDVisualChangeVideoFormat,
SNDVisualUpdateAudio,
SNDVisualGetAudioSpace,
SNDVisualMuteAudio,
SNDVisualUnMuteAudio,
SNDVisualSetVolume
};
