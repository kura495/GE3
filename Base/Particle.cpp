#include "Particle.h"
Particle::Particle(){}
Particle::~Particle(){}

void Particle::Initalize()
{
	modelData.vertices.push_back{position = {1.0f,1.0f,0.0f,1.0f},.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f}};//����
	modelData.vertices.push_back{.position = {},.texcoord = {},.normal = {}};//�E��
	modelData.vertices.push_back{.position = {},.texcoord = {},.normal = {}};//����
	modelData.vertices.push_back{.position = {},.texcoord = {},.normal = {}};//����
	modelData.vertices.push_back{.position = {},.texcoord = {},.normal = {}};//�E��
	modelData.vertices.push_back{.position = {},.texcoord = {},.normal = {}};//�E��
}

void Particle::Update()
{

}

void Particle::Draw()
{

}
