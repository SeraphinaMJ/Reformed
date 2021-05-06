#pragma once
#include "Audio/AudioComponent.hpp"
#include "Components/FBXFileComponent.h"
#include "Components/AnimatorComponent.hpp"
#include "Components/Agent.hpp"
#include "GameScripts/AudioScript.h"
#include "GameScripts/BombScript.h"
#include "Components/Button.hpp"
#include "Components/Camera.hpp"
#include "GameScripts/CameraScript.hpp"
#include "GameScripts/ChangeSceneAfterDelayScript.hpp"
#include "GameScripts/CharacterStateScript.h"
#include "GameScripts/DamagePrismScript.hpp"
#include "GameScripts/DynamiteScript.h"
#include "GameScripts/ExitGameButton.hpp"
#include "GameScripts/EnemyAIScript.h"
#include "GameScripts/GameManagerScript.h"
#include "GameScripts/GrabbableScript.hpp"
#include "GameScripts/GroundPoundScript.h"
#include "Components/Health.hpp"
//#include "Components/Allegiance.hpp"
#include "Components/InputComponent.hpp"
#include "GameScripts/InputTestScript.hpp"
#include "Replay/JumbotronComponent.hpp"
#include "GameScripts/JumpPadScript.h"
#include "GameScripts/LariatScript.h"
#include "GameScripts/LifetimeScript.hpp"
#include "Components/Light.hpp"
#include "GameScripts/LightTestScript.hpp"
#include "GameScripts/menuControllerScript.hpp"
#include "GameScripts/MouseLookScript.hpp"
#include "Components/ParticleEmitter.hpp"
#include "GameScripts/PikminPlayerControllerScript.hpp"
#include "GameScripts/PlayButtonScript.hpp"
#include "GameScripts/PlayerControllerScript.hpp"
#include "Components/RenderTarget.hpp"
#include "GameScripts/RenderTargetScript.hpp"
#include "Components/Renderer.hpp"
#include "Components/RigidBody.hpp"
#include "GameScripts/ShooterScript.h"
#include "Components/Skybox.hpp"
#include "GameScripts/SpawnerScript.h"
#include "GameScripts/StressTestScript.hpp"
#include "GameScripts/TestingScript.hpp"
#include "GameScripts/TextSampleScript.hpp"
#include "GameScripts/ThirdPersonCameraController.hpp"
#include "SceneManagement/Transform.hpp"
#include "TransformTestScript.hpp"
#include "DebugLines.hpp"
#include "Components/WaypointNode.hpp"
#include "Components/WaypointNodeSet.hpp"
#include "GameScripts/TargetingCircleScript.hpp"
#include "GameScripts/PathFollowScript.hpp"
#include "Components/UISprite.hpp"
#include "Components/UISpriteArray.hpp"
#include "Components/Enemy.hpp"
#include "Components/Player.hpp"
#include "GameScripts/RequestConfirmation.hpp"
#include "GameScripts/KillFieldScript.hpp"

#include "MinionGroup_1.hpp"
#include "MinionGroup_2.hpp"
#include "MinionGroup_3.hpp"
#include "MinionGroup_4.hpp"
#include "MinionGroup_5.hpp"
#include "MinionGroup_6.hpp"
#include "MinionGroup_7.hpp"
#include "MinionGroup_8.hpp"
#include "MinionGroup_9.hpp"
#include "MinionGroup_10.hpp"

#include "MinionType_1.hpp"
#include "MinionType_2.hpp"
#include "MinionType_3.hpp"

#include "MinionAll.hpp"
#include "MinionAllExceptType_1.hpp"

#include "MinionLeader.hpp"

#include "GameScripts/ToggleDisplayOnButtonPress.hpp"
#include "GameScripts/ScoreTracker.hpp"
#include "GameScripts/CheatCodeScript.hpp"

#define COMPONENT_LIST \
X(Agent)\
X(Audio::AudioComponent)\
X(FBXFileComponent)\
X(animatorComponent)\
X(audioScript)\
X(bombScript)\
X(button)\
X(camera)\
X(cameraScript)\
X(changeSceneAfterDelayScript)\
X(characterStateScript)\
X(cheatCodeScript)\
X(damagePrismScript)\
X(dynamiteScript)\
X(debugLines)\
X(enemyAIScript)\
X(enemy)\
X(exitGameButtonScript)\
X(gameManagerScript)\
X(grabbableScript)\
X(groundPoundScript)\
X(Health)\
X(inputComponent)\
X(inputTestScript)\
X(jumbotronComponent)\
X(jumpPadScript)\
X(killFieldScript)\
X(lariatScript)\
X(lifetimeScript)\
X(light)\
X(lightTestScript)\
X(menuControllerScript)\
X(minionAll)\
X(minionAllExceptType_1)\
X(minionGroup_1)\
X(minionGroup_2)\
X(minionGroup_3)\
X(minionGroup_4)\
X(minionGroup_5)\
X(minionGroup_6)\
X(minionGroup_7)\
X(minionGroup_8)\
X(minionGroup_9)\
X(minionGroup_10)\
X(minionLeader)\
X(minionType_1)\
X(minionType_2)\
X(minionType_3)\
X(mouseLookScript)\
X(particleEmitter)\
X(pikminPlayerControllerScript)\
X(playButtonScript)\
X(playerControllerScript)\
X(pathFollowScript)\
X(player)\
X(renderTarget)\
X(renderTargetScript)\
X(renderer)\
X(requestConfirmation)\
X(rigidBody)\
X(scoreTracker)\
X(shooterScript)\
X(skybox)\
X(spawnerScript)\
X(stressTestScript)\
X(testingScript)\
X(textSampleScript)\
X(thirdPersonCameraController)\
X(transform)\
X(transformTestScript)\
X(targetingCircleScript)\
X(toggleDisplayOnButtonPress)\
X(uiSprite)\
X(uiSpriteArray)\
X(waypointNode)\
X(waypointNodeSet)\



#define SCRIPT_LIST \
X(audioScript)\
X(bombScript)\
X(cameraScript)\
X(changeSceneAfterDelayScript)\
X(characterStateScript)\
X(cheatCodeScript)\
X(damagePrismScript)\
X(dynamiteScript)\
X(exitGameButtonScript)\
X(enemyAIScript)\
X(gameManagerScript)\
X(grabbableScript)\
X(groundPoundScript)\
X(inputTestScript)\
X(jumpPadScript)\
X(killFieldScript)\
X(lariatScript)\
X(lifetimeScript)\
X(lightTestScript)\
X(menuControllerScript)\
X(mouseLookScript)\
X(pikminPlayerControllerScript)\
X(playButtonScript)\
X(playerControllerScript)\
X(renderTargetScript)\
X(requestConfirmation)\
X(scoreTracker)\
X(shooterScript)\
X(spawnerScript)\
X(stressTestScript)\
X(testingScript)\
X(textSampleScript)\
X(thirdPersonCameraController)\
X(toggleDisplayOnButtonPress)\
X(transformTestScript)\
X(targetingCircleScript)\
X(pathFollowScript)\

