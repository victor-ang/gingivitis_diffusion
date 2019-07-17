#ifndef GINGICELL_H_
#define GINGICELL_H_
#include "../plugin/delaunaymassspringdamper/BodyDelaunayMassSpringDamper.hpp"
#include "../plugin/diffusion/BodyDiffusion.hpp"
#include "Bodies3D.hpp"
#include "GingiScenario.hpp"
#include <mecacell/mecacell.h>

enum SIGNAL { INFLAMMATORY = 0, RESOLUTIVE = 1, EATME = 2 };

template <template <typename> class B>
class GingiCell : public MecaCell::ConnectableCell<GingiCell<B>, B> {
  using Base = MecaCell::ConnectableCell<GingiCell<B>, B>;

public:
  // Different states a cell can have
  enum State {
    Alive,
    Apoptosis, // death without inflammation
    Necrosis // death with inflammation
  };

  // Cell types 
  enum Type {
    Immune,
    Stroma,
    ImmuneMaker // adapt: number of cells
  };

  enum ImmuneType { None, Resident, Circulatory };

  std::uniform_real_distribution<float> dice;

  nlohmann::json *config;
  float speed; // cell movement speed
  float divisionProb; // probability of cell division
  float deathProb; // probability of apoptosis
  float health; // life of the cell
  float killing; // ability to eliminate cells in necrosis or apoptosis
  State state;
  float inflaProd;   // pro-inflammatory production
  float resoProd;    // pro-resolutive production
  float inflaDegrad; // decrease in pro-inflammatory
  float resoDegrad;  // decrease in pro-resolutive
  float inflaHealthImpact; // impact of inflammation on healt
  float extinctionProb; // probability of disappearance of circulating cells after resolution of the inflammation
  Type type;
  ImmuneType immuneType;
  int age; // cell age


  // ASK
  GingiCell(const MecaCell::Vec &v)
      : Base(), dice(0.0f, 1.0f) // Uniform (0,1)
  {
    this->body.moveTo(v);
  }

  GingiCell(GingiCell<B> *mother) : Base(mother->getPosition()) {
    this->body.init(mother);
  }

  void init(GingiCell<B> *mother) {

    this->speed = mother->speed;
    this->divisionProb = mother->divisionProb;
    this->deathProb = mother->deathProb;
    this->health = mother->health;
    this->killing = mother->killing;
    this->state = mother->state;
    this->inflaProd = mother->inflaProd;
    this->resoProd = mother->resoProd;
    this->inflaDegrad = mother->inflaDegrad;
    this->resoDegrad = mother->resoDegrad;
    this->inflaHealthImpact = mother->inflaHealthImpact;
    this->setColor(mother->color[0], mother->color[1], mother->color[2]);
    this->type = mother->type;
    this->immuneType = mother->immuneType;
    this->age = mother->age;
    this->setVisible(mother->getVisible());
  }

  void init(Type type, nlohmann::json *js) { // Json added
    config = js;
    if (type == Immune) {
      this->age = 0;
      this->speed = (*config)["Immune"]["speed"];
      this->divisionProb = (*config)["Immune"]["divisionProb"];
      this->deathProb = (*config)["Immune"]["deathProb"];
      this->health = (*config)["Immune"]["health"];
      this->killing = (*config)["Immune"]["killing"];
      this->state = Alive;
      this->inflaProd = (*config)["Immune"]["inflaProd"];
      this->resoProd = (*config)["Immune"]["resoProd"];
      this->inflaDegrad = (*config)["Immune"]["inflaDegrad"];
      this->resoDegrad = (*config)["Immune"]["resoDegrad"];
      this->inflaHealthImpact = (*config)["Immune"]["inflaHealthImpact"];
      this->setColor((float)((*config)["Immune"]["red"]),
                (float)((*config)["Immune"]["green"]),
                (float)((*config)["Immune"]["blue"]));
      this->type = Immune;
      this->immuneType = Resident;
    } else if (type == Stroma) {
      this->age = 0;
      this->speed = (*config)["Stroma"]["speed"];
      this->divisionProb = (*config)["Stroma"]["divisionProb"];
      this->deathProb = (*config)["Stroma"]["deathProb"];
      this->health = (*config)["Stroma"]["health"];
      this->killing = (*config)["Stroma"]["killing"];
      this->state = Alive;
      this->inflaProd = (*config)["Stroma"]["inflaProd"];
      this->resoProd = (*config)["Stroma"]["resoProd"];
      this->inflaDegrad = (*config)["Stroma"]["inflaDegrad"];
      this->resoDegrad = (*config)["Stroma"]["resoDegrad"];
      this->inflaHealthImpact = (*config)["Stroma"]["inflaHealthImpact"];
      this->setColor((float)((*config)["Stroma"]["red"]),
                (float)((*config)["Stroma"]["green"]),
                (float)((*config)["Stroma"]["blue"]));
      this->type = Stroma;
      this->immuneType = None;
    } else if (type == ImmuneMaker) {
      this->age = 0;
      this->speed = (*config)["ImmuneMaker"]["speed"];
      this->divisionProb = (*config)["ImmuneMaker"]["divisionProb"];
      this->deathProb = (*config)["ImmuneMaker"]["deathProb"];
      this->health = (*config)["ImmuneMaker"]["health"];
      this->killing = (*config)["ImmuneMaker"]["killing"];
      this->state = Alive;
      this->inflaProd = (*config)["ImmuneMaker"]["inflaProd"];
      this->resoProd = (*config)["ImmuneMaker"]["resoProd"];
      this->inflaDegrad = (*config)["ImmuneMaker"]["inflaDegrad"];
      this->resoDegrad = (*config)["ImmuneMaker"]["resoDegrad"];
      this->inflaHealthImpact = (*config)["ImmuneMaker"]["inflaHealthImpact"];
      this->setColor((float)((*config)["ImmuneMaker"]["red"]),
                (float)((*config)["ImmuneMaker"]["green"]),
                (float)((*config)["ImmuneMaker"]["blue"]));
      this->type = ImmuneMaker;
      this->immuneType = None;
    }
  }

  inline double getAdhesion() { return 1.; }

  // adhesion coefficient with other cells 
  inline double getAdhesionWith(const GingiCell *, MecaCell::Vec) {
    return 1.25;
  }

  template <class W> void updateBehavior(W &w) {
    this->age++;

    // Constraining cells in the box
    constrainingCells();

    // Immune maker behavior
    if (this->type == ImmuneMaker) {
      immuneMakerBehavior(w);
      return;
    }


    // Death
    if (state == Apoptosis || state == Necrosis) {
      disappearance(state);
      return;
    }
    
    // Programmed death
    if (dice(MecaCell::Config::globalRand()) < deathProb) { // random number between 0 and 1
      apoptosis();
      return;
    }

    // Impact of inflammation on health
    //std::cerr << this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] << std::endl;
    this->health -= this->inflaHealthImpact * this->getBody().getQuantities()[SIGNAL::INFLAMMATORY];
    //std::cerr <<this->health << std::endl;
    if (this->health <= 0) {
      necrosis();
      return;
    }

    // Killing
    for (GingiCell<B> *c : this->getConnectedCells()) {
      if (kill(c) == true) {
        return;
      }
    }

    // PI + PR
    double avgInfla = 0.0;
    updateSignal(&avgInfla, w);

    // Disappearance of Circulatory cells after resolution of the inflammation
    disappearanceCirculatory(avgInfla);

    MecaCell::Vec dir;

    // Eat me
    if (eatme(dir, w) == true) {
      return;
    }

    // Inflation
    if (moveToMaxInflaCell(dir) == true) {
      return;
    }

    // Random motion
    randomMotion(dir);

    // Division
    if (division(w) == true) {
      return;
    }
  }

  GingiCell<B> *divide() {
    // reset current cell
    // this->body.setRadius(20.); //(this->body.getBaseRadius())
    // this->body.setMass(this->body.getBaseMass());

    MecaCell::Vec randomVec = MecaCell::Vec::randomUnit();
    MecaCell::Vec daughterPostion(this->getPosition() + randomVec * MecaCell::Config::DEFAULT_CELL_RADIUS * 0.75);
    GingiCell<B> *daughter = new GingiCell<B>(daughterPostion);

    this->age = 0; // age of the new cell (daughter) after division
    daughter->init(this);

    return daughter;
  }

/*   inline MecaCell::Vec getRealPosition() {
    return this->getPosition() / 4. * 0.82;
  }

  inline double getRealBoundingBoxRadius() {
    return this->getBoundingBoxRadius() / 4. * 0.82;
  } */

  void setColor(double r, double g, double b) {
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
  }







  // FUNCTIONS USED IN updateBehavior

  template <class W> void immuneMakerBehavior(W &w) {
    // Number of ImmuneMaker cells : uniform distribution between 0 and 1 // PLUTOT UNE NORMALE
    // You can change the minimum and maximum values in j.json file (lowValue and highValue)


    // Division proba
    if (dice(MecaCell::Config::globalRand()) < divisionProb * this->getBody().getQuantities()[SIGNAL::INFLAMMATORY]*10) {
	  // The more inflammation there is, the more likely it is that the cell will divide
      immuneCirculatoryCreation(w);
    }

    setColor(1.0, 0.5 + 0.5 * this->getBody().getQuantities()[SIGNAL::INFLAMMATORY], this->getBody().getQuantities()[SIGNAL::INFLAMMATORY]);
  }

  template <class W> void immuneCirculatoryCreation(W &w) {

    // Creation of Circulatory Immune cells :
    //	- Do not divide
    // 	- Do not die of apoptosis
    //	- Move faster than Resident cells
    //	- Produce more inflammation than Residents cells
    //	- Pro-resolutive after the switch
    //	- Disappear when there is no more inflammation

    GingiCell<B> *c = divide();
    c->type = Immune;
    c->init(Immune, config);
    c->immuneType = Circulatory; // Circulatory cells created by ImmuneMaker
    c->speed = (float)((*config)["ImmuneCirculatory"]["speed"]); // Circulatory move faster
    c->divisionProb = (float)((*config)["ImmuneCirculatory"]["divisionProb"]);
    c->deathProb = (float)((*config)["ImmuneCirculatory"]["deathProb"]);
    c->inflaHealthImpact = (float)((*config)["ImmuneCirculatory"]["inflaHealthImpact"]);
    c->inflaProd = (float)((*config)["ImmuneCirculatory"]["inflaProd"]); // Circulatory immune cell much more infla than the resident
    c->resoProd = (float)((*config)["ImmuneCirculatory"]["resoProd"]);
    c->resoDegrad = (float)((*config)["ImmuneCirculatory"]["resoDegrad"]);
    c->inflaDegrad = (float)((*config)["ImmuneCirculatory"]["inflaDegrad"]);
    c->extinctionProb = (*config)["ImmuneCirculatory"]["extinctionProb"];
    c->setColor((float)((*config)["ImmuneCirculatory"]["red"]),
                (float)((*config)["ImmuneCirculatory"]["green"]),
                (float)((*config)["ImmuneCirculatory"]["blue"]));
    w.addCell(c);
  }

  void constrainingCells() {

    MecaCell::Vec pos = MecaCell::Vec(this->getPosition());
    float minPos = -250.0f; //- GingiScenario::BOX_HALF_SIZE;
    float maxPos = 250.0f;  // GingiScenario::BOX_HALF_SIZE;
    if (pos.x() < minPos)
      pos.setX(minPos);
    if (pos.x() > maxPos)
      pos.setX(maxPos);
    if (pos.y() < minPos)
      pos.setY(minPos);
    if (pos.y() > maxPos)
      pos.setY(maxPos);
    if (pos.z() < minPos)
      pos.setZ(minPos);
    if (pos.z() > maxPos)
      pos.setZ(maxPos);
    this->body.moveTo(pos);
  }

  void disappearance(State state) {

    if (state == Apoptosis) {
      this->setColor(health, health, health);
      if (health <= 0.0) {
        this->die();
      }
    } else if (state == Necrosis) {
      this->setVisible(true);
      this->setColor(health, 0.0, health);
      if (health <= 0.0) {
        this->die();
      }
    }
  }

  void apoptosis() {

    state = Apoptosis;
    this->setVisible(true);
    this->setColor(health, health, health);
  }

  void necrosis() {

    this->health = 1;
    this->state = Necrosis;
    this->setColor(health, 0.0, health);
  }

  template <class W> bool division(W &w) {
    bool b = false;
    double avgDist = 0.0;
    for (GingiCell<B> *c : this->getConnectedCells()) {
      avgDist += (this->getPosition() - c->getPosition()).length();
    }
    avgDist /= this->getConnectedCells().size();
    if (avgDist > 1.5 * this->getBoundingBoxRadius()) { // Average distance greater than 1.5
                                           			        // times the radius of a cell

      if (dice(MecaCell::Config::globalRand()) < divisionProb) {
        GingiCell<B> *daughter = divide();
        w.addCell(daughter);
        b = true;
      }
    }
    return b;
  }

  bool kill(GingiCell<B> *c) {
    bool b = false;
    if (c->state == Apoptosis) {
      if (type == Immune)
        this->setColor(1.0, 0.5, 0.5);
      else if (type == Stroma)
        this->setColor(0.5, 0.5, 1.0);
      c->health -= killing;
      b = true;

    } else if (c->state == Necrosis) {
      if (type == Immune)
        this->setColor(this->getBody().getQuantities()[SIGNAL::INFLAMMATORY], 0.0, this->getBody().getQuantities()[SIGNAL::INFLAMMATORY]);
      else if (type == Stroma)
        this->setColor(0.5 + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5, 0.5 + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5, 0.0);
      c->getBody().setConsumption(SIGNAL::INFLAMMATORY, -inflaProd * c->health); // propagation depends on the life of the necrotic cell
      c->health -= killing;
    }
    return b;
  }

  template <class W> void updateSignal(double *avgInfla, W &w) {

     for (auto *c : this->getConnectedCells()) {
      *avgInfla = max(*avgInfla + c->getBody().getQuantities()[SIGNAL::INFLAMMATORY] / this->getConnectedCells().size() -
                          c->getBody().getQuantities()[SIGNAL::RESOLUTIVE] / this->getConnectedCells().size(),(double)0);
     }

    // While the cell feels eat-me produced by necrotic cells,
    // its inflammation increases and its resolutive decreases.
    // When it no longer feels eat-me from necrotic cells : switch (its resolutive
    // increases and its inflammation decreases.)

    float closestNecroDist = 1000000; // infinity
    GingiCell<B> *closestNecroCell = nullptr;
    for (GingiCell<B> *cell : w.cells) {
      if (!cell->isDead() && cell != this) {
        if (cell->state == Necrosis)  {
          cell->getBody().setConsumption(SIGNAL::EATME, -1.0 * cell->health); // la cellule en nécrose émet du eat-me propotiinnellement a sa vie
          MecaCell::Vec dis = this->getPosition() - cell->getPosition();
          double len = dis.length();
          if (len < closestNecroDist) {
            closestNecroDist = len;
            closestNecroCell = cell;
          }
        }
      }
    }
    // std::cerr <<closestNecroDist <<std::endl;

    if (closestNecroCell) {
      if (this->getBody().getQuantities()[SIGNAL::EATME] >= 0) {
        this->getBody().setConsumption(SIGNAL::INFLAMMATORY, -inflaProd);
        this->getBody().setConsumption(SIGNAL::RESOLUTIVE, resoDegrad);
      }
      else {
        if (type == Immune) { // the immune cells switch first
          this->getBody().setConsumption(SIGNAL::INFLAMMATORY, inflaDegrad);
          this->getBody().setConsumption(SIGNAL::RESOLUTIVE, -resoProd);
        }
      }
    } else if (!closestNecroCell && *avgInfla > 0.0) {
      this->getBody().setConsumption(SIGNAL::INFLAMMATORY, inflaDegrad);
      this->getBody().setConsumption(SIGNAL::RESOLUTIVE, -resoProd);
    }

    this->getBody().setConsumption(SIGNAL::INFLAMMATORY, inflaDegrad);
    this->getBody().setConsumption(SIGNAL::RESOLUTIVE, resoDegrad);
    if (this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] > 0.0f)
      this->setColor(0.5f + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5f, 0.5f + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5f, 0.0);
    else if (type == Immune)
      this->setColor(1.0, 0.5, 0.5);
    else if (type == Stroma)
      this->setColor(0.5, 0.5, 1.0);
  }


  void disappearanceCirculatory(double avgInfla) {
    if (avgInfla <= 0) {
      if (this->immuneType == Circulatory && dice(MecaCell::Config::globalRand()) < extinctionProb) {
        this->die();
      }
    }
  }

  bool moveToMaxInflaCell(MecaCell::Vec dir) {
    bool b = false;
    double maxInfla = 0;
    GingiCell<B> *maxInflaCell = nullptr;
    for (auto *c : this->getConnectedCells()) {
      if (!c->isDead()) {
        if (c->getBody().getQuantities()[SIGNAL::INFLAMMATORY] > maxInfla) {
          maxInfla = c->getBody().getQuantities()[SIGNAL::INFLAMMATORY];
          maxInflaCell = c;
        }
      }
    }
    if (maxInflaCell && maxInfla > 0.5) {
      dir = this->getPosition() - maxInflaCell->getPosition();
      dir /= (dir.length() + 0.001);
      this->setColor(0.5 + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5, 0.5 + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5, 0.0);
      dir *= speed;
      this->body.moveTo(this->getPosition() - dir);
      b = true;
    }
    return b;
  }

  template <class W> bool eatme(MecaCell::Vec dir, W &w) {
    bool b = false;
    float closestApopDist = 1000000; // infty
    GingiCell<B> *closestApopCell = nullptr;
    for (GingiCell<B> *c : w.cells) {
      if (!c->isDead() && c != this) {
        if (c->state == Apoptosis || c->state == Necrosis) {
          c->getBody().setConsumption(SIGNAL::EATME, -1.0 * c->health); // la cellule en nécrose ou en apoptose émet du eat-me
          MecaCell::Vec d = this->getPosition() - c->getPosition();
          double l = d.length();
          if (l < closestApopDist) {
            closestApopDist = l;
            closestApopCell = c;
          }
        }
      }
    }
    // std::cerr <<closestApopDist <<std::endl;

    if (closestApopCell) {
      if (this->getBody().getQuantities()[SIGNAL::EATME] <= 0) {
        dir = this->getPosition() - closestApopCell->getPosition();
        dir /= (closestApopDist + 0.001);
        // this->setColor(0.0, 1.0, 0.0);
        dir *= speed;
        this->body.moveTo(this->getPosition() - dir);
        b = true;
      }
    }
    return b;
  }


  void randomMotion(MecaCell::Vec dir) {
    dir = MecaCell::Vec::randomUnit();
    dir *= speed;
    this->body.moveTo(this->getPosition() - dir);
  }
};

#endif
