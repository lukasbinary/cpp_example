// Functional.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <optional>
#include <functional>
#include <iterator>

struct Point
{
	float _x{ 0.0f };
	float _y{ 0.0f };
};

class Monster
{
public:
	Monster(int32_t id, int32_t hp, int32_t armor, const Point& pt) : _id(id), _hp(hp), _armor(armor), _pt(pt) { std::cout << "Monster()" << std::endl; }
	~Monster() { std::cout << "~Monster()" << std::endl; }

	int32_t GetID() const { return _id; }
	int32_t GetHP() const { return _hp; }
	int32_t GetArmor() const { return _armor; }
	Point GetLocation() const { return _pt; }

	static int32_t calc_damaged_hp(int32_t hp, int32_t damage, int32_t armor)
	{
		return hp - std::min(hp, damage - armor);
	}

private:
	int32_t _id{ 0 };
	int32_t _hp{ 0 };
	int32_t _armor{ 0 };
	Point _pt{ 0.0f, 0.0f };
};

int32_t calc_distance(const Point& pt1, const Point& pt2)
{
	return static_cast<int32_t>(sqrtf((pt1._x - pt2._x) * (pt1._x - pt2._x) + (pt1._y - pt2._y) * (pt1._y - pt2._y)));
}

auto is_in_range_(const Point& my_location, int32_t range_from, int32_t range_to)
{
	return [my_location, range_from, range_to](const std::shared_ptr<Monster>& pm)
	{
		int32_t distance = calc_distance(my_location, pm->GetLocation());
		return distance >= range_from && distance <= range_to;
	};
}

auto damaged_monster_(int32_t damage)
{
	return [damage](const std::shared_ptr<Monster>& pm)
	{
		return std::make_shared<Monster>(pm->GetID(),
			Monster::calc_damaged_hp(pm->GetHP(), damage, pm->GetArmor()),
			pm->GetArmor(),
			pm->GetLocation());
	};
}

auto do_damage_(const Point& my_location, int32_t damage, int32_t range_from, int32_t range_to)
{
	return [my_location, damage, range_from, range_to](const std::shared_ptr<Monster>& pm)
	{
		return is_in_range_(my_location, range_from, range_to)(pm) ? damaged_monster_(damage)(pm) : pm;
	};
}

bool is_alive(const std::shared_ptr<Monster>& pm)
{
	return pm->GetHP() > 0;
}

void doSplashDamage_stl(const Point& my_location, std::vector<std::shared_ptr<Monster>>& monsters, int32_t damage, int32_t range_from, int32_t range_to)
{
	std::transform(std::begin(monsters), std::end(monsters), std::begin(monsters),
		do_damage_(my_location, damage, range_from, range_to));
	auto is_dead = [](const auto& m)
	{
		return is_alive(m) == false;
	};
	monsters.erase(std::remove_if(std::begin(monsters), std::end(monsters), is_dead), std::end(monsters));
}

template<typename A>
std::optional<A> unit_(A a)
{
	return std::make_optional<A>(a);
}

template<typename F>
std::optional<std::string> bind_(const std::optional<std::string>& v, F func)
{
	return v ? func(*v) : std::nullopt;
}

template<typename A, typename B, typename C>
std::function<C(A)> compose(std::function<C(B)> f, std::function<B(A)> g)
{
	return [f, g](A x)
	{
		return f(g(x));
	};
}

struct Creature
{
	Creature(int64_t id, int32_t hp) : _id(id), _hp(hp) {}
	~Creature() { std::cout << "Dead CreatureId : " << _id << std::endl; }
	int64_t _id{ 0 };
	int32_t _hp{ 0 };
};

auto IsAlive(const std::shared_ptr<Creature>& creature)
{
	std::cout << "CreatureId : " << creature->_hp << std::endl;
	return creature->_hp % 2 == 0 ? creature : std::make_shared<Creature>(creature->_id, creature->_hp * 2);
}

int main()
{
	std::vector<std::shared_ptr<Creature>> creatures
	{
		std::make_shared<Creature>(0, 0),
		std::make_shared<Creature>(1, 1),
		std::make_shared<Creature>(2, 2),
		std::make_shared<Creature>(3, 3),
		std::make_shared<Creature>(4, 4),
		std::make_shared<Creature>(5, 5),
		std::make_shared<Creature>(6, 6),
		std::make_shared<Creature>(7, 7),
		std::make_shared<Creature>(8, 8),
		std::make_shared<Creature>(9, 9),
	};
	std::vector<std::shared_ptr<Creature>> updatedCreatures;

	//std::transform(std::begin(creatures), std::end(creatures), std::begin(creatures), IsAlive);
	std::transform(std::begin(creatures), std::end(creatures), std::back_inserter(updatedCreatures), IsAlive);
	std::wcout << "----------------------------------------------------------------" << std::endl;
	std::for_each(std::begin(creatures), std::end(creatures), [](const auto& creature)
	{
		std::cout << "CreatureId : " << creature->_id << " CreatureHp : " << creature->_hp << std::endl;
	});
	std::wcout << "----------------------------------------------------------------" << std::endl;
	std::for_each(std::begin(updatedCreatures), std::end(updatedCreatures), [](const auto& creature)
	{
		std::cout << "CreatureId : " << creature->_id << " CreatureHp : " << creature->_hp << std::endl;
	});
	std::wcout << "----------------------------------------------------------------" << std::endl;
	creatures = std::move(updatedCreatures);

    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
