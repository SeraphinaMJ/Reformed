/*!***************************************************************************************
\file       GameObjectTests.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "gtest/gtest.h"
#include "SceneManagement/GameObject.hpp"
#include "SceneManagement/Transform.hpp"
#include <MathFunctions.hpp>
#include <ComponentRepository.hpp> // componentRepository

class GameObjectTest : public ::testing::Test
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

TEST_F(GameObjectTest, Construction)
{
    std::shared_ptr<gameObject> obj = theSpace->instantiateGameObject();
    ASSERT_NE(obj, nullptr);
    ASSERT_EQ(theSpace->getObjectCount(), 1);
    theSpace->destroyGameObject(obj);
    ASSERT_EQ(theSpace->getObjectCount(), 0);
}

TEST_F(GameObjectTest, Parenting)
{
	std::shared_ptr<gameObject> parent_obj = theSpace->instantiateGameObject();
	std::shared_ptr<gameObject> child_obj = theSpace->instantiateGameObject();
	componentHandle<transform> l_parent_trans = parent_obj->getComponent<transform>();
	componentHandle<transform> l_child_trans = child_obj->getComponent<transform>();

	// check parenting properties
	EXPECT_EQ(parent_obj->getChildren().size(), 0);
	EXPECT_EQ(child_obj->getParent().expired(), true);

	// setup transforms
	l_parent_trans->setPosition(vector3(12, 23, 34));
	l_parent_trans->setRotation(toQuaternion(vector3(45, 56, 67)));
	l_parent_trans->setScale(vector3(78, 89, 90));
	l_child_trans->setPosition(vector3(112, 123, 134));
	l_child_trans->setRotation(toQuaternion(vector3(145, 156, 167)));
	l_child_trans->setScale(vector3(178, 189, 190));

	parent_obj->addChild(child_obj);

	// check parenting properties
	EXPECT_EQ(parent_obj->getChildren().size(), 1);
	EXPECT_EQ(child_obj->getParent().expired(), false);
	EXPECT_EQ(child_obj->getParent().lock(), parent_obj);

	// check transforms
	EXPECT_EQ(l_parent_trans->getPosition(), vector3(12, 23, 34));
	EXPECT_EQ(l_parent_trans->getRotation(), toQuaternion(vector3(45, 56, 67)));
	EXPECT_EQ(l_parent_trans->getScale(), vector3(78, 89, 90));
	EXPECT_NEAR(l_child_trans->getPosition().x, -0.122825277792f, 0.0000001f);
	EXPECT_FLOAT_EQ(l_child_trans->getPosition().y, 0.887048036766f);
	EXPECT_FLOAT_EQ(l_child_trans->getPosition().z, 1.709651043949f);
	EXPECT_FLOAT_EQ(l_child_trans->getRotation().x, -0.342874761495f);
	EXPECT_FLOAT_EQ(l_child_trans->getRotation().y, -0.411683123637f);
	EXPECT_FLOAT_EQ(l_child_trans->getRotation().z, -0.156817874637f);
	EXPECT_FLOAT_EQ(l_child_trans->getRotation().w, 0.829675875168f );
	EXPECT_FLOAT_EQ(l_child_trans->getScale().x, 2.282051282051f);
	EXPECT_FLOAT_EQ(l_child_trans->getScale().y, 2.123595505618f);
	EXPECT_FLOAT_EQ(l_child_trans->getScale().z, 2.111111111111f);
	EXPECT_FLOAT_EQ(l_child_trans->getGlobalPosition().getX(), 112);
	EXPECT_FLOAT_EQ(l_child_trans->getGlobalPosition().getY(), 123);
	EXPECT_FLOAT_EQ(l_child_trans->getGlobalPosition().getZ(), 134);
	EXPECT_EQ(l_child_trans->getGlobalRotation(), toQuaternion(vector3(145, 156, 167)).Conjugated());
	EXPECT_EQ(l_child_trans->getGlobalScale().getX(), 178);
	EXPECT_EQ(l_child_trans->getGlobalScale().getY(), 189);
	EXPECT_EQ(l_child_trans->getGlobalScale().getZ(), 190);

}

TEST_F(GameObjectTest, Unparenting)
{

}
