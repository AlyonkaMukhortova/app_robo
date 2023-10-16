#pragma once

enum class EnvErr
{
	MoveProblem,
	ManageProblem,
	PosEnergyProblem,
	NegEnergyProblem
};

enum class NodeTypes
{
	ManNode,
	GenNode,
	SenseNode
};

enum Direction
{
	Up, Right, Down, Left
};

enum class ComponentTypes
{
	ManRobot = 0,
	ScRobot = 1,
	ManCentre = 2,
	ObsCentre = 3
};

enum class ObjType
{
	Obstacle,
	SmthInteresting,
	MyPlatform
};


enum class Instructions {
	ReconnectNewMan,
	ReconnectNewPlat,
	AddModule,
	AddGenerator,
	ModuleOn,
	OnGenerator,
	TurnVector
};

enum Angles {
	A0,
	A90,
	A180,
	A270,
	A360
};
