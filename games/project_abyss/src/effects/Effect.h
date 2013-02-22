// project_abyss
// LittleSpace Studio 2012

#ifndef MK_EFFECT
#define MK_EFFECT

class Effect
{
	public:
		Effect() { mTimer = 0.0f, isActive = false; };
		
		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Draw(float) = 0;
		virtual void StartEffect() = 0;
		virtual void StopEffect() = 0;
		
		bool GetActive() { return isActive; };

	protected:
		float mTimer;
		bool isActive;

};

#endif