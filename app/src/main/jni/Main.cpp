/*
 * Credits:
 *
 * Octowolve - Mod menu: https://github.com/z3r0Sec/Substrate-Template-With-Mod-Menu
 * And hooking: https://github.com/z3r0Sec/Substrate-Hooking-Example
 * VanHoevenTR A.K.A Nixi: https://github.com/LGLTeam/VanHoevenTR_Android_Mod_Menu
 * MrIkso - Mod menu: https://github.com/MrIkso/FloatingModMenu
 * Rprop - https://github.com/Rprop/And64InlineHook
 * MJx0 A.K.A Ruit - KittyMemory: https://github.com/MJx0/KittyMemory
 * */
#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include "Includes/obfuscate.h"
#include "KittyMemory/MemoryPatch.h"
#include "Includes/Logger.h"
#include "Includes/Utils.h"
#include "Menu.h"
#include <Substrate/SubstrateHook.h>
#include <Substrate/CydiaSubstrate.h>
#include <iostream>

// fancy struct for patches for kittyMemory
struct My_Patches {
MemoryPatch MapHackIcon;
MemoryPatch MapHackNoIcon;
MemoryPatch VisibleInGrass;
MemoryPatch UnlockSkin;
MemoryPatch UnlockSkin2;

} hexPatches;
bool MapHackIcon = false;
bool MapHackNoIcon = false;
bool VisibleInGrass = false;
bool UnlockSkin = false;

//Target lib here
#define targetLibName OBFUSCATE("liblogic.so")

extern "C" {
JNIEXPORT void JNICALL
Java_fang_cybertron_MainActivity_Toast(JNIEnv *env, jclass obj, jobject context) {
    MakeToast(env, context, OBFUSCATE("Modded by FangCybertron\nTelegram : https://t.me/FangCybertron"), Toast::LENGTH_LONG);
}

JNIEXPORT jobjectArray
JNICALL
Java_fang_cybertron_modmenu_FloatingModMenuService_getFeatureList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;
    const char *features[] = {
            OBFUSCATE("Category_MENU"),
            OBFUSCATE("1_Toggle_MapHack Icon"),
			OBFUSCATE("2_Toggle_MapHack No Icon (Lobby)"),
            OBFUSCATE("3_Toggle_Visible In Grass"),
			OBFUSCATE("4_Toggle_Unlock All Skins"),
			OBFUSCATE("4_Toggle_Unlock Up Comming Skins"),
			};

    //Now you dont have to manually update the number everytime;
    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));
    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    pthread_t ptid;
    pthread_create(&ptid, NULL, antiLeech, NULL);
    return (ret);
}

JNIEXPORT void JNICALL
Java_fang_cybertron_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint featNum, jstring featName, jint value,
                                        jboolean boolean, jstring str) {
    //Convert java string to c++
    const char *featureName = env->GetStringUTFChars(featName, 0);
    const char *TextInput;
    if (str != NULL)
        TextInput = env->GetStringUTFChars(str, 0);
    else
        TextInput = "Empty";

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d | Text: = %s"), featNum,
         featureName, value,
         boolean, TextInput);

    //BE CAREFUL NOT TO ACCIDENTLY REMOVE break;

    switch (featNum) {
        case 1:
	        MapHackIcon = boolean;
            if (MapHackIcon) {
                hexPatches.MapHackIcon.Modify();
                LOGI(OBFUSCATE("On"));
            } else {
                hexPatches.MapHackIcon.Restore();
                LOGI(OBFUSCATE("Off"));
            }
            break;
		case 2:
			MapHackNoIcon = boolean;
            if (MapHackNoIcon) {
                hexPatches.MapHackNoIcon.Modify();
                LOGI(OBFUSCATE("On"));
            } else {
                hexPatches.MapHackNoIcon.Restore();
                LOGI(OBFUSCATE("Off"));
            }
            break;
		case 3:
			VisibleInGrass = boolean;
            if (VisibleInGrass) {
                hexPatches.VisibleInGrass.Modify();
                LOGI(OBFUSCATE("On"));
            } else {
                hexPatches.VisibleInGrass.Restore();
                LOGI(OBFUSCATE("Off"));
            }
            break;
		case 4:
			UnlockSkin = boolean;
            if (UnlockSkin) {
                hexPatches.UnlockSkin.Modify();
				hexPatches.UnlockSkin2.Modify();
                LOGI(OBFUSCATE("On"));
            } else {
                hexPatches.UnlockSkin.Restore();
				hexPatches.UnlockSkin2.Restore();
                LOGI(OBFUSCATE("Off"));
            }
            break;
    }
  }
}

// Hooking example


void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread called"));
    //Check if target lib is loaded
    do {
        sleep(1);
    } while (!isLibraryLoaded(targetLibName));

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);

hexPatches.MapHackIcon = MemoryPatch::createWithHex(targetLibName, //BUseEsportsEmblem
                                                    string2Offset(OBFUSCATE("0x4A87AD4")),
                                                    OBFUSCATE("01 00 A0 E1 1E FF 2F E1"));
													
hexPatches.MapHackIcon = MemoryPatch::createWithHex(targetLibName, //BUseEsportsEmblem
                                                    string2Offset(OBFUSCATE("0x4A87AD4")),
                                                    OBFUSCATE("01 00 A0 E1 1E FF 2F E1"));
													
hexPatches.VisibleInGrass = MemoryPatch::createWithHex(targetLibName, //BUseEsportsEmblem
                                                    string2Offset(OBFUSCATE("0x4A87AD4")),
                                                    OBFUSCATE("01 00 A0 E1 1E FF 2F E1"));
													
hexPatches.UnlockSkin = MemoryPatch::createWithHex(targetLibName, //BUseEsportsEmblem
                                                    string2Offset(OBFUSCATE("0x4A87AD4")),
                                                    OBFUSCATE("01 00 A0 E1 1E FF 2F E1"));
													
hexPatches.UnlockSkin2 = MemoryPatch::createWithHex(targetLibName, //BUseEsportsEmblem
                                                    string2Offset(OBFUSCATE("0x4A87AD4")),
                                                    OBFUSCATE("01 00 A0 E1 1E FF 2F E1"));
	
LOGI(OBFUSCATE("Done"));
return NULL;
}

//No need to use JNI_OnLoad, since we don't use JNIEnv
//We do this to hide OnLoad from disassembler
__attribute__((constructor))
void lib_main() {
    // Create a new thread so it does not block the main thread, means the game would not freeze
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}

/*
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *globalEnv;
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);

    return JNI_VERSION_1_6;
}
 */
