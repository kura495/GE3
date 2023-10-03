#include "Particle.h"
Particle::Particle(){}
Particle::~Particle(){}

void Particle::Initalize()
{
	modelData.vertices.push_back{position = {1.0f,1.0f,0.0f,1.0f},.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f}};//ç∂è„
	modelData.vertices.push_back{.position = {},.texcoord = {},.normal = {}};//âEè„
	modelData.vertices.push_back{.position = {},.texcoord = {},.normal = {}};//ç∂â∫
	modelData.vertices.push_back{.position = {},.texcoord = {},.normal = {}};//ç∂è„
	modelData.vertices.push_back{.position = {},.texcoord = {},.normal = {}};//âEè„
	modelData.vertices.push_back{.position = {},.texcoord = {},.normal = {}};//âEâ∫
}

void Particle::Update()
{

}

void Particle::Draw()
{

}
