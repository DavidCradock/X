#pragma once
// Master include file for X library
#include "PCH.h"

// 2D
#include "2DCamera.h"
#include "2DEntity.h"
#include "2DEntityLine.h"
#include "2DEntityRot.h"
#include "2DLayer.h"
#include "2DMap.h"
#include "2DMapTile.h"
#include "2DMapImageType.h"
#include "2DParticle.h"
#include "2DParticleAffector.h"
#include "2DParticleEmitter.h"
#include "2DParticleSystem.h"
#include "2DParticleType.h"
#include "2DRenderer.h"
#include "2DWorld.h"
// AI
#include "finiteStateMachine.h"
#include "geneticAlgorithm.h"
#include "neuralNetFeedForward.h"
// Applications
#include "applicationBase.h"
#include "applicationManager.h"
// Audio
#include "audioManager.h"
// Core
#include "colour.h"
#include "image.h"
#include "imageAtlas.h"
#include "log.h"
#include "openGLExtensions.h"
#include "precision.h"
#include "settings.h"
#include "singleton.h"
#include "singletons.h"
#include "stringUtils.h"
#include "timer.h"
#include "utilities.h"
#include "window.h"
// GUI
#include "GUIBaseObject.h"
#include "GUIButton.h"
#include "GUIButtonImage.h"
#include "GUICheckbox.h"
#include "GUIContainer.h"
#include "GUIDraggableDock.h"
#include "GUIDraggableItem.h"
#include "GUIImage.h"
#include "GUIImageAnimated.h"
#include "GUIImageDepthbuffer.h"
#include "GUIImageFramebuffer.h"
#include "GUILineGraph.h"
#include "GUIManager.h"
#include "GUIMenu.h"
#include "GUIProgressBar.h"
#include "GUISlider.h"
#include "GUITab.h"
#include "GUITabContainer.h"
#include "GUITaskbar.h"
#include "GUIText.h"
#include "GUITextEdit.h"
#include "GUITextScroll.h"
#include "GUITheme.h"
#include "GUITooltip.h"
// Input
#include "input.h"
#include "inputJoystick.h"
#include "inputKeyboard.h"
#include "inputMouse.h"
// Math
#include "AABB.h"
#include "frustum.h"
#include "matrix.h"
#include "plane.h"
#include "quaternion.h"
#include "rect.h"
#include "vector2d.h"
#include "vector2f.h"
#include "vector2r.h"
#include "vector3d.h"
#include "vector3f.h"
#include "vector3r.h"
#include "vector4f.h"
// Physics
#include "physicsManager.h"
// Resources
#include "resourceBase.h"
#include "resourceDepthbuffer.h"
#include "resourceFont.h"
#include "resourceFramebuffer.h"
#include "resourceLoadingScreen.h"
#include "resourceManager.h"
#include "resourceShader.h"
#include "resourceTexture2DAtlas.h"
#include "resourceTexture2DFromFile.h"
#include "resourceTexture2DFromImage.h"
#include "resourceVertexBufferCPT.h"
#include "resourceVertexBufferCPT2.h"
#include "resourceVertexBufferCPTBNT.h"
#include "resourceVertexBufferCPTInst.h"
#include "resourceVertexBufferLine.h"
// Scene manager
#include "SMCamera.h"
#include "SMEntityBase.h"
#include "SMEntityLine.h"
#include "SMEntityVertexBuffer.h"
#include "SMLightDirectional.h"
#include "SMLightPoint.h"
#include "SMMaterial.h"
#include "SMSimple.h"
// Spatial partitioning
#include "octTree.h"
#include "quadTree.h"
#include "quadTreeEntity.h"
#include "quadTreeNode.h"
