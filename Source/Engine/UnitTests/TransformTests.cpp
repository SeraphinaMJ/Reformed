/*!***************************************************************************************
\file       TransformTests.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#define GLM_ENABLE_EXPERIMENTAL

#include "gtest/gtest.h"
#include "SceneManagement/Transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "PrintHelpers.hpp"
#include "MathFunctions.hpp"
#include <Vector3Serialization.hpp>
#include <QuaternionSerialization.hpp>
#include <ComponentRepository.hpp> // componentRepository


TEST(Transform, Construction)
{
	transform * l_trans = new transform();

	ASSERT_EQ(l_trans->getPosition(), vector3(0, 0, 0));
	ASSERT_EQ(l_trans->getRotation(), quaternion(0, 0, 0, 1));
	ASSERT_EQ(l_trans->getScale(), vector3(1, 1, 1));

	delete l_trans;

	transform l_trans2;

	ASSERT_EQ(l_trans2.getPosition(), vector3(0, 0, 0));
	ASSERT_EQ(l_trans2.getRotation(), quaternion(0, 0, 0, 1));
	ASSERT_EQ(l_trans2.getScale(), vector3(1, 1, 1));
}

/*
TEST(Transform, ComponentBasics)
{
	transform l_trans;

	ASSERT_STREQ(transform::getType().name().c_str(), "class transform");
	ASSERT_STREQ(l_trans.type().name().c_str(), "class transform");
	ASSERT_EQ(transform::getType() == l_trans.type(), true);
}
*/

TEST(Transform, PositionGettersSettersOffsetters)
{
	transform l_trans;

	ASSERT_EQ(l_trans.getPosition(), vector3(0, 0, 0));

	l_trans.setPosition(vector3(12, 23, 34));

	ASSERT_EQ(l_trans.getPosition(), vector3(12, 23, 34));

	l_trans.setPosX(112);
	l_trans.setPosY(123);
	l_trans.setPosZ(134);

	ASSERT_EQ(l_trans.getPosition(), vector3(112, 123, 134));

	l_trans.offsetPosition(vector3(-100, -100, -100));

	ASSERT_EQ(l_trans.getPosition(), vector3(12, 23, 34));

	l_trans.offsetPosX(200);
	l_trans.offsetPosY(200);
	l_trans.offsetPosZ(200);

	ASSERT_EQ(l_trans.getPosition(), vector3(212, 223, 234));
}

TEST(Transform, RotationGettersSettersOffsetters)
{
	// #python program to validate numbers
	//def run() :
	//	initialEulers = numpy.array([12, 23, 34])
	//	offsetEulers = numpy.array([-100, -100, -100])
	//	xoffsetRads = 200
	//	yoffsetRads = 200
	//	zoffsetRads = 200
	//
	//	initialRads = numpy.array([initialEulers[0] / 180 * numpy.pi, initialEulers[1] / 180 * numpy.pi, initialEulers[2] / 180 * numpy.pi])
	//	initial = quaternion_from_euler(initialRads[0], initialRads[1], initialRads[2], 'sxyz')
	//	print("Initial = " + displayQuaternion(initial))
	//
	//	offsetRads = numpy.array([offsetEulers[0] / 180 * numpy.pi, offsetEulers[1] / 180 * numpy.pi, offsetEulers[2] / 180 * numpy.pi])
	//	offset = quaternion_from_euler(offsetRads[0], offsetRads[1], offsetRads[2], 'sxyz')
	//	print("Offset  = " + displayQuaternion(offset))
	//
	//	result = quaternion_multiply(offset, initial)
	//	print("Result  = " + displayQuaternion(result))
	//
	//	#xoffsetRads = xoffsetEuler / 180 * numpy.pi
	//	xoffsetQuat = quaternion_from_euler(xoffsetRads, 0, 0, 'sxyz')
	//	#yoffsetRads = yoffsetEuler / 180 * numpy.pi
	//	yoffsetQuat = quaternion_from_euler(0, yoffsetRads, 0, 'sxyz')
	//	#zoffsetRads = zoffsetEuler / 180 * numpy.pi
	//	zoffsetQuat = quaternion_from_euler(0, 0, zoffsetRads, 'sxyz')
	//
	//	result2x = quaternion_multiply(xoffsetQuat, result)
	//	result2y = quaternion_multiply(yoffsetQuat, result2x)
	//	result2z = quaternion_multiply(zoffsetQuat, result2y)
	//	print("Result2 = " + displayQuaternion(result2z))
	//
	//
	//def displayQuaternion(quat) :
	//	return "[x: {0:15.12f}, y: {1:15.12f}, z: {2:15.12f}, w: {3:15.12f}]".format(quat[1], quat[2], quat[3], quat[0]);

	transform l_trans;
	EXPECT_EQ(l_trans.getRotation(), quaternion(0, 0, 0, 1));

	l_trans.setRotation(toQuaternion(vector3(12, 23, 34)));
	EXPECT_EQ(l_trans.getRotation(), toQuaternion({ 12, 23, 34 }));

	l_trans.offsetRotation(toQuaternion(vector3(-100, -100, -100)));
	const quaternion l_expected(
		-0.48970952609100737f,
		0.17264481120654968f,
		-0.8542354705599029f,
		0.025692607039322413f);
	EXPECT_FLOAT_EQ(l_trans.getRotation().x, l_expected.x);
	EXPECT_FLOAT_EQ(l_trans.getRotation().y, l_expected.y);
	EXPECT_FLOAT_EQ(l_trans.getRotation().z, l_expected.z);
	EXPECT_NEAR(l_trans.getRotation().w, l_expected.w, 0.0000001f);

	l_trans.offsetRotX(200);
	l_trans.offsetRotY(200);
	l_trans.offsetRotZ(200);
	const quaternion l_expected2(
		-0.029833028089f,
		-0.133559442161f,
		-0.631485772249f,
		-0.763215294194f);
	EXPECT_NEAR(l_trans.getRotation().x, l_expected2.x, 0.000001f);
	EXPECT_FLOAT_EQ(l_trans.getRotation().y, l_expected2.y);
	EXPECT_FLOAT_EQ(l_trans.getRotation().z, l_expected2.z);
	EXPECT_FLOAT_EQ(l_trans.getRotation().w, l_expected2.w);
}

TEST(Transform, ScaleGettersSettersOffsetters)
{
	transform l_trans;

	ASSERT_EQ(l_trans.getScale(), vector3(1, 1, 1));

	l_trans.setScale(vector3(12, 23, 34));

	ASSERT_EQ(l_trans.getScale(), vector3(12, 23, 34));

	l_trans.setScaleX(112);
	l_trans.setScaleY(123);
	l_trans.setScaleZ(134);

	ASSERT_EQ(l_trans.getScale(), vector3(112, 123, 134));

	l_trans.offsetScale(vector3(-100, -100, -100));

	ASSERT_EQ(l_trans.getScale(), vector3(12, 23, 34));

	l_trans.offsetScaleX(200);
	l_trans.offsetScaleY(200);
	l_trans.offsetScaleZ(200);

	ASSERT_EQ(l_trans.getScale(), vector3(212, 223, 234));
}

//TEST(Transform, TypeRTConversion)
//{
//	transform l_trans;
//	typeRT l_rt = l_trans.toTypeRT();
//
//	ASSERT_STREQ(l_rt.getTypeName().c_str(), "transform");
//	ASSERT_STREQ(l_rt.getVariableName().c_str(), "transform");
//	ASSERT_EQ(l_rt.members().size(), 3);
//
//	ASSERT_EQ(l_rt.members().find("position") != l_rt.members().end(), true);
//	typeRT l_posRT = l_rt.member("position");
//	ASSERT_EQ(l_posRT, vector3(0, 0, 0).toTypeRT());
//	ASSERT_EQ(l_rt.members().find("rotation") != l_rt.members().end(), true);
//	typeRT l_rotRT = l_rt.member("rotation");
//	ASSERT_EQ(l_rotRT, vector3(0, 0, 0).toTypeRT());
//	ASSERT_EQ(l_rt.members().find("scale") != l_rt.members().end(), true);
//	typeRT l_scaleRT = l_rt.member("scale");
//	ASSERT_EQ(l_scaleRT, vector3(1, 1, 1).toTypeRT());
//}

//TEST(Transform, TypeRTConstruction)
//{
//	typeRT l_rt("transform");
//	l_rt.setTypeName("transform");
//	typeRT l_posRT = vector3(12, 23, 34).toTypeRT();
//	l_posRT.setVariableName("position");
//	l_rt.insertMember(l_posRT);
//	typeRT l_rotRT = vector3(45, 56, 67).toTypeRT();
//	l_rotRT.setVariableName("rotation");
//	l_rt.insertMember(l_rotRT);
//	typeRT l_scaleRT = vector3(78, 89, 90).toTypeRT();
//	l_scaleRT.setVariableName("scale");
//	l_rt.insertMember(l_scaleRT);
//
//	transform l_trans(l_rt);
//	ASSERT_EQ(l_trans.getPosition(), vector3(12,23,34));
//    ASSERT_EQ(l_trans.getRotation(), toQuaternion({ 45, 56, 67}));
//	ASSERT_EQ(l_trans.getScale(), vector3(78, 89, 90));
//}

TEST(Transform, Matrix4x4Construction)
{
	typeRT l_rt("class transform");

	matrix4x4 l_mat = matrix4x4::build(vector3(12, 23, 34), toQuaternion(vector3(45, 56, 67)), vector3(78, 89, 90));

	transform l_trans;
	l_trans.fromLocalMatrix(l_mat);


	ASSERT_STREQ(l_rt.getTypeName().c_str(), "");
	ASSERT_STREQ(l_rt.getVariableName().c_str(), "class transform");
	ASSERT_EQ(l_rt.members().size(), 3);

	quaternion l_expected_rotation = toQuaternion({ 45,56,67 });
	EXPECT_FLOAT_EQ(l_trans.getRotation().getX(), l_expected_rotation.getX());
	EXPECT_FLOAT_EQ(l_trans.getRotation().getY(), l_expected_rotation.getY());
	EXPECT_FLOAT_EQ(l_trans.getRotation().getZ(), l_expected_rotation.getZ());
	EXPECT_FLOAT_EQ(l_trans.getRotation().getW(), l_expected_rotation.getW());

	EXPECT_FLOAT_EQ(l_trans.getScale().getX(), 78);
	EXPECT_FLOAT_EQ(l_trans.getScale().getY(), 89);
	EXPECT_FLOAT_EQ(l_trans.getScale().getZ(), 90);
}

TEST(Transform, GetLocalMatrix)
{
	transform l_trans;
	l_trans.setPosition(vector3(12, 23, 34));
	l_trans.setRotation(toQuaternion(vector3(45, 56, 67)));
	l_trans.setScale(vector3(78, 89, 90));

	matrix4x4 l_mat = l_trans.getLocalMatrix();

	matrix4x4 l_expected_mat = matrix4x4::build(vector3(12, 23, 34),
	                                            toQuaternion(vector3(45, 56, 67)), vector3(78, 89, 90));
	EXPECT_EQ(l_mat, l_expected_mat);
}

TEST(Transform, GetGlobalMatrix)
{

	typeRT l_rt("class transform");
	l_rt.setTypeName("class transform");
	typeRT l_posRT = ::toTypeRT(vector3(12, 23, 34));
	l_posRT.setVariableName("position");
	l_rt.insertMember(l_posRT);
	typeRT l_rotRT = ::toTypeRT(toQuaternion(vector3(45, 56, 67)));
	l_rotRT.setVariableName("rotation");
	l_rt.insertMember(l_rotRT);
	typeRT l_scaleRT = ::toTypeRT(vector3(78, 89, 90));
	l_scaleRT.setVariableName("scale");
	l_rt.insertMember(l_scaleRT);

	//transform l_trans = matrix4x4::build(vector3(12, 23, 34), toQuaternion(vector3(45, 56, 67)), vector3(78, 89, 90));

	//matrix4x4 l_mat = l_trans.getGlobalMatrix();

	matrix4x4 l_expected_mat = matrix4x4::build(vector3(12, 23, 34),
	                                            toQuaternion(vector3(45, 56, 67)), vector3(78, 89, 90));
	//EXPECT_EQ(l_mat, l_expected_mat);
}


class TransformWithParenting : public ::testing::Test
{
protected:
	std::unique_ptr<gameObjectCollector> goc;
	std::shared_ptr<scene> theScene;
	std::shared_ptr<space> theSpace;

	void SetUp() override
	{
		goc = std::make_unique<gameObjectCollectorMock>();
		theScene = std::make_shared<scene>(goc.get());
		theSpace = theScene->createSpace();
		EXPECT_NE(theSpace, nullptr);
	}

	class gameObjectCollectorMock : public gameObjectCollector
	{
		public:
			gameObjectCollectorMock(): m_component_repo(new componentRepository())
			{ }

		private:
			void updateGameObject(std::shared_ptr<gameObject> /*p_game_object*/, GameObjectFiltering::componentTypeSet const& /*p_previous_componentTypeSet*/) override
			{
				// do nothing
			}

			icomponentRepository * getComponentRepository() override
			{
				// do nothing
				return m_component_repo;
			}

			icomponentRepository * m_component_repo;
	};

	//void TearDown() override
	//{
	//	// nothing to do
	//}
};

TEST_F(TransformWithParenting, GlobalMatrixCalculation)
{
	std::shared_ptr<gameObject> l_child = theSpace->instantiateGameObject();
	componentHandle<transform> l_child_trans = l_child->getComponent<transform>();

	l_child_trans->setPosition(vector3(12, 23, 34));
	l_child_trans->setRotation(toQuaternion({ 45,56,67 }));
	l_child_trans->setScale(vector3(78, 89, 90));

	const matrix4x4 l_local_mat = l_child_trans->getLocalMatrix();
	const matrix4x4 l_global_mat = l_child_trans->getGlobalMatrix();

	const matrix4x4 l_expected_mat = matrix4x4::build(vector3(12, 23, 34), toQuaternion({ 45,56,67 }), vector3(78, 89, 90));

	EXPECT_EQ(l_local_mat, l_expected_mat);
	EXPECT_EQ(l_global_mat, l_expected_mat);
}

TEST_F(TransformWithParenting, ParentGlobalMatrixConcatenation)
{
	std::shared_ptr<gameObject> l_parent = theSpace->instantiateGameObject();
	std::shared_ptr<gameObject> l_child = theSpace->instantiateGameObject();
	l_parent->addChild(l_child);

	componentHandle<transform> l_parent_trans = l_parent->getComponent<transform>();
	componentHandle<transform> l_child_trans = l_child->getComponent<transform>();

	l_parent_trans->setPosition(vector3(12, 23, 34));
	l_parent_trans->setRotation(toQuaternion({ 45, 56, 67 }));
	l_parent_trans->setScale(vector3(78, 89, 90));
	l_child_trans->setPosition(vector3(1, 2, 3));
	l_child_trans->setRotation(toQuaternion({ 103, 104, 105 }));
	l_child_trans->setScale(vector3(2, 3, 4));

	const matrix4x4 l_parent_local_mat = l_parent_trans->getLocalMatrix();
	const matrix4x4 l_parent_global_mat = l_parent_trans->getGlobalMatrix();
	const matrix4x4 l_child_local_mat = l_child_trans->getLocalMatrix();
	const matrix4x4 l_child_global_mat = l_child_trans->getGlobalMatrix();

	const matrix4x4 l_expected_parent_mat = matrix4x4::build(
		vector3(12, 23, 34), toQuaternion({ 45,56,67 }), vector3(78, 89, 90));
	const matrix4x4 l_expected_child_local_mat = matrix4x4::build(
		vector3(1, 2, 3), toQuaternion({ 103,104,105 }), vector3(2, 3, 4));
	const matrix4x4 l_expected_mat = //l_expected_child_local_mat * l_expected_parent_mat;
		l_expected_parent_mat * l_expected_child_local_mat;

	// verify the parent's local matrix built properly
	EXPECT_EQ(l_parent_local_mat, l_expected_parent_mat);
	// verify the parent's global matrix built properly
	EXPECT_EQ(l_parent_global_mat, l_expected_parent_mat);
	// verify the child local matrix built properly
	EXPECT_EQ(l_child_local_mat, l_expected_child_local_mat);
	// verify the global matrix of the child is the concatenation of the parent global and child local
	EXPECT_EQ(l_child_global_mat, l_expected_mat);
}

TEST_F(TransformWithParenting, ChildDoesNotMoveOnParenting)
{
	std::shared_ptr<gameObject> l_parent = theSpace->instantiateGameObject();
	std::shared_ptr<gameObject> l_child = theSpace->instantiateGameObject();

	componentHandle<transform> l_parent_trans = l_parent->getComponent<transform>();
	componentHandle<transform> l_child_trans = l_child->getComponent<transform>();

	l_parent_trans->setPosition(vector3(112, 123, 134));
	l_parent_trans->setRotation(toQuaternion(vector3(0, 0, 0)));
	l_parent_trans->setScale(vector3(1, 1, 1));
	l_child_trans->setPosition(vector3(12, 23, 34));
	l_child_trans->setRotation(toQuaternion(vector3(0, 0, 0)));
	l_child_trans->setScale(vector3(1, 1, 1));

	// parent the child
	l_parent->addChild(l_child);

	// verify globals didn't change
	EXPECT_FLOAT_EQ(l_child->getComponent<transform>()->getGlobalPosition().x, 12.0f);
	EXPECT_FLOAT_EQ(l_child->getComponent<transform>()->getGlobalPosition().y, 23.0f);
	EXPECT_FLOAT_EQ(l_child->getComponent<transform>()->getGlobalPosition().z, 34.0f);

	// verify locals are calculated correctly
	EXPECT_FLOAT_EQ(l_child->getComponent<transform>()->getPosition().x, -100.0f);
	EXPECT_FLOAT_EQ(l_child->getComponent<transform>()->getPosition().y, -100.0f);
	EXPECT_FLOAT_EQ(l_child->getComponent<transform>()->getPosition().z, -100.0f);

}
TEST_F(TransformWithParenting, ChildDoesNotRotateOnParenting)
{
	std::shared_ptr<gameObject> l_parent = theSpace->instantiateGameObject();
	std::shared_ptr<gameObject> l_child = theSpace->instantiateGameObject();

	componentHandle<transform> l_parent_trans = l_parent->getComponent<transform>();
	componentHandle<transform> l_child_trans = l_child->getComponent<transform>();

	l_parent_trans->setPosition(vector3(0, 0, 0));
	l_parent_trans->setRotation(toQuaternion(vector3(145, 156, 167)));
	l_parent_trans->setScale(vector3(1, 1, 1));
	l_child_trans->setPosition(vector3(0, 0, 0));
	l_child_trans->setRotation(toQuaternion(vector3(45, 56, 67)));
	l_child_trans->setScale(vector3(1, 1, 1));

	// parent the child
	l_parent->addChild(l_child);

	// verify globals didn't change
	quaternion l_expected_quat = toQuaternion(vector3(45, 56, 67));
	EXPECT_FLOAT_EQ(l_child_trans->getGlobalRotation().x, l_expected_quat.x);
	EXPECT_FLOAT_EQ(l_child_trans->getGlobalRotation().y, l_expected_quat.y);
	EXPECT_FLOAT_EQ(l_child_trans->getGlobalRotation().z, l_expected_quat.z);
	EXPECT_FLOAT_EQ(l_child_trans->getGlobalRotation().w, l_expected_quat.w);

	// verify locals are calculated correctly
	quaternion l_local_rot = l_child->getComponent<transform>()->getRotation();
	quaternion l_expected_local_rot = l_child->getComponent<transform>()->getGlobalRotation() * l_parent->getComponent<transform>()->getGlobalRotation().Conjugated();
	EXPECT_FLOAT_EQ(l_local_rot.x, l_expected_local_rot.x);
	EXPECT_FLOAT_EQ(l_local_rot.y, l_expected_local_rot.y);
	EXPECT_FLOAT_EQ(l_local_rot.z, l_expected_local_rot.z);
	EXPECT_FLOAT_EQ(l_local_rot.w, l_expected_local_rot.w);

}

TEST_F(TransformWithParenting, ChildDoesNotScaleOnParenting)
{
	std::shared_ptr<gameObject> l_parent = theSpace->instantiateGameObject();
	std::shared_ptr<gameObject> l_child = theSpace->instantiateGameObject();

	componentHandle<transform> l_parent_trans = l_parent->getComponent<transform>();
	componentHandle<transform> l_child_trans = l_child->getComponent<transform>();

	l_parent_trans->setPosition(vector3(0, 0, 0));
	l_parent_trans->setRotation(toQuaternion(vector3(0, 0, 0)));
	l_parent_trans->setScale(vector3(178, 189, 190));
	l_child_trans->setPosition(vector3(0, 0, 0));
	l_child_trans->setRotation(toQuaternion(vector3(0, 0, 0)));
	l_child_trans->setScale(vector3(78, 89, 90));

	// parent the child
	l_parent->addChild(l_child);

	// verify globals didn't change
	EXPECT_FLOAT_EQ(l_child->getComponent<transform>()->getGlobalScale().x, 78.0f);
	EXPECT_FLOAT_EQ(l_child->getComponent<transform>()->getGlobalScale().y, 89.0f);
	EXPECT_FLOAT_EQ(l_child->getComponent<transform>()->getGlobalScale().z, 90.0f);

	// verify locals are calculated correctly
	EXPECT_FLOAT_EQ(l_child->getComponent<transform>()->getScale().x, 78.0f / 178.0f);
	EXPECT_FLOAT_EQ(l_child->getComponent<transform>()->getScale().y, 89.0f / 189.0f);
	EXPECT_FLOAT_EQ(l_child->getComponent<transform>()->getScale().z, 90.0f / 190.0f);

}
