#ifndef GINGICELL_H_
#define GINGICELL_H_
#include "../plugin/delaunaymassspringdamper/BodyDelaunayMassSpringDamper.hpp"
#include "../plugin/diffusion/BodyDiffusion.hpp"
#include "Bodies3D.hpp"
#include "GingiScenario.hpp"
#include <mecacell/mecacell.h>

// namespace Diffusion {

enum SIGNAL { INFLAMMATORY = 0, RESOLUTIVE = 1, EATME = 2 };

template <template <typename> class B>
class GingiCell : public MecaCell::ConnectableCell<GingiCell<B>, B> {
  using Base = MecaCell::ConnectableCell<GingiCell<B>, B>;

public:
  // Different states a cell can have
  enum State {
    Alive,
    Apoptosis, // death without inflammation
    Necrosis   // death with inflammation
  };

  // Cell types
  enum Type {
    Immune,
    Stroma,
    ImmuneMaker // number of immunemaker : normal distribution
  };

  enum ImmuneType {
    None,
    Resident,
    Circulatory
  };

  std::uniform_real_distribution<float> dice;

  nlohmann::json *config;
  float speed;        // cell movement speed
  float divisionProb; // probability of cell division
  float deathProb;    // probability of apoptosis
  float health;       // life of the cell
  float killing;      // ability to eliminate cells in necrosis or apoptosis
  State state;
  float inflaProd;         // pro-inflammatory production
  float resoProd;          // pro-resolutive production
  float inflaDegrad;       // decrease in pro-inflammatory
  float resoDegrad;        // decrease in pro-resolutive
  float inflaHealthImpact; // impact of inflammation on healt
  float extinctionProb;    // probability of disappearance of circulating cells
                           // after resolution of the inflammation
  Type type;
  ImmuneType immuneType;
  int age;            // cell age
  float eatenCounter; // counter of what the cell has eaten
  int shiftStep;      // step for the shift M1/M2

  GingiCell(const MecaCell::Vec &v, DiffusionGrid *g)
      : Base(v), dice(0.0f, 1.0f) {
    // this->body.moveTo(v);
    this->getBody().setGrid(g);
  }

  GingiCell(GingiCell<B> *mother) : Base(mother->getPosition()) {
    this->body.setGrid(mother->getBody().getGrid());
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
    this->eatenCounter = mother->eatenCounter;
    this->shiftStep = mother->shiftStep;
    this->setVisible(mother->getVisible());
  }

  char *typeToString(Type type) {
    if (type == Immune) {
      return "Immune";
    } else if (type == Stroma) {
      return "Stroma";
    } else if (type == ImmuneMaker) {
      return "ImmuneMaker";
    }
  }

  char *immuneTypeToString(ImmuneType immuneType) {
    if (immuneType == None) {
      return "None";
    } else if (immuneType == Resident) {
      return "Resident";
    } else if (immuneType == Circulatory) {
      return "Circulatory";
    }
  }

  void assignParameters(Type type, ImmuneType immuneType, nlohmann::json *js) {
    config = js;
    this->age = 0;
    this->eatenCounter = 0.0;
    this->shiftStep = (*config)[typeToString(type)][immuneTypeToString(immuneType)]["shiftStep"];
    this->speed = (*config)[typeToString(type)][immuneTypeToString(immuneType)]["speed"];
    this->divisionProb = (*config)[typeToString(type)][immuneTypeToString(immuneType)]["divisionProb"];
    this->deathProb = (*config)[typeToString(type)][immuneTypeToString(immuneType)]["deathProb"];
    this->health = (*config)[typeToString(type)][immuneTypeToString(immuneType)]["health"];
    this->killing = (*config)[typeToString(type)][immuneTypeToString(immuneType)]["killing"];
    this->state = Alive;
    this->inflaProd = (*config)[typeToString(type)][immuneTypeToString(immuneType)]["inflaProd"];
    this->resoProd = (*config)[typeToString(type)][immuneTypeToString(immuneType)]["resoProd"];
    this->inflaDegrad = (*config)[typeToString(type)][immuneTypeToString(immuneType)]["inflaDegrad"];
    this->resoDegrad = (*config)[typeToString(type)][immuneTypeToString(immuneType)]["resoDegrad"];
    this->inflaHealthImpact = (*config)[typeToString(type)][immuneTypeToString(immuneType)]["inflaHealthImpact"];
    this->extinctionProb = (*config)[typeToString(type)][immuneTypeToString(immuneType)]["extinctionProb"];
    this->setColor(
        ((*config)[typeToString(type)][immuneTypeToString(immuneType)]["red"]),
        ((*config)[typeToString(type)][immuneTypeToString(immuneType)]["green"]),
        ((*config)[typeToString(type)][immuneTypeToString(immuneType)]["blue"]));
    this->immuneType = immuneType;
    this->type = type;
  }

  void init(Type type, ImmuneType immuneType, nlohmann::json *js) {
    config = js;
    if (type == Immune) {
      if (immuneType == Resident) {
        assignParameters(Immune, Resident, js);
      } else if (immuneType == Circulatory) {
        assignParameters(Immune, Circulatory, js);
      }
    } else if (type == Stroma) {
      assignParameters(Stroma, None, js);
    } else if (type == ImmuneMaker) {
      assignParameters(ImmuneMaker, None, js);
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
    }
    else if (this->state == Apoptosis) {
      eatme();
      disappearance(Apoptosis); // Death
      moving();
    }
    else if (this->state == Necrosis) {
      eatme();
      disappearance(Necrosis);
      updateSignal(); // PR + PI
      moving();


    } else if (this->state == Alive) {
      bool actionDone = false;
      signalsRelay();
      moving();
      actionDone = toApoptosis();
      if (!actionDone) {
        actionDone = toNecrosis();
      }
      if (!actionDone) {
        actionDone = eat();
      }
      if (!actionDone) {
        actionDone = shift();
      }
      if (!actionDone) {
        actionDone = division(w);
      }
      // Disappearance of Circulatory cells after resolution of the inflammation
      disappearanceCirculatory();
    }
  }

    
    
    
    
    
  // FUNCTIONS USED IN updateBehavior

  GingiCell<B> *divide() {
    // reset current cell
    // this->body.setRadius(20.); //(this->body.getBaseRadius())
    // this->body.setMass(this->body.getBaseMass());

    MecaCell::Vec randomVec = MecaCell::Vec::randomUnit();
    // MecaCell::Vec daughterPostion(this->getPosition() + randomVec *
    // MecaCell::Config::DEFAULT_CELL_RADIUS * 0.75);
    MecaCell::Vec daughterPostion(
        this->getPosition() + randomVec * this->getBoundingBoxRadius() * 0.75);

    GingiCell<B> *daughter =
        new GingiCell<B>(daughterPostion, this->getBody().getGrid());

    this->age = 0; // age of the new cell (daughter) after division
    daughter->init(this);

    return daughter;
  }

  void setColor(double r, double g, double b) {
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
  }

  template <class W> void immuneMakerBehavior(W &w) {
    // Number of ImmuneMaker cells : normal distribution
    // Mean and the variance values in j.json file
    //if (dice(MecaCell::Config::globalRand()) < divisionProb * this->getBody().getQuantities()[SIGNAL::INFLAMMATORY]) {
    if (dice(MecaCell::Config::globalRand()) < 1.0) {
      // The more inflammation there is, the more likely it is that the cell
      // will divide
      immuneCirculatoryCreation(w);
    }
    setColor(1.0, 0.5 + 0.5 * this->getBody().getQuantities()[SIGNAL::INFLAMMATORY],this->getBody().getQuantities()[SIGNAL::INFLAMMATORY]);
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
    c->init(Immune, Circulatory, config);
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

    if (health <= 0.0) {
      this->die();
    }
    if (state == Apoptosis) {
      this->setColor(health, health, health);
    } else if (state == Necrosis) {
      this->setVisible(true);
      this->setColor(health, 0.0, health);
    }
  }

  bool toApoptosis() {
    bool apoptosis = false;
    if (dice(MecaCell::Config::globalRand()) < deathProb) { // random number between 0 and 1
      state = Apoptosis;
      this->setVisible(true);
      this->setColor(health, health, health);
      apoptosis = true;
    }
    return apoptosis;
  }

  bool toNecrosis() {
    bool necrosis = false;
    // Impact of inflammation on health
    this->health -= this->inflaHealthImpact * this->getBody().getQuantities()[SIGNAL::INFLAMMATORY];
    if (this->health <= 0) {
      this->health = 1;
      this->state = Necrosis;
      this->setColor(health, 0.0, health);
      necrosis = true;
    }
    return necrosis;
  }

  template <class W> bool division(W &w) {
    bool hasDivided = false;
    double avgDist = 0.0;
    for (GingiCell<B> *c : this->getConnectedCells()) {
      avgDist += (this->getPosition() - c->getPosition()).length();
    }
    avgDist /= this->getConnectedCells().size();
    if (avgDist > 1.5 * this->getBoundingBoxRadius()) { // Average distance greater than 1.5
                                                        // times the radius of a cell
      if (dice(MecaCell::Config::globalRand()) < divisionProb * health) {
        if (!this->isDead()) {
          GingiCell<B> *daughter = divide();
          w.addCell(daughter);
          hasDivided = true;
        }
      }
    }
    return hasDivided;
  }

  void signalsRelay() {
    double sumInfla = 0.0;
    double sumReso = 0.0;
    for (GingiCell<B> *c : this->getConnectedCells()) {
      sumInfla += c->getBody().getQuantities()[SIGNAL::INFLAMMATORY];
      sumReso += c->getBody().getQuantities()[SIGNAL::RESOLUTIVE];
    }
    if (sumInfla > sumReso) {
        this->getBody().setConsumption(SIGNAL::INFLAMMATORY, -inflaProd * this->getBody().getQuantities()[SIGNAL::INFLAMMATORY]);
    }
    else {
      this->getBody().setConsumption(SIGNAL::RESOLUTIVE, -resoProd * this->getBody().getQuantities()[SIGNAL::RESOLUTIVE]);
    }
  }

  bool eat() {
    bool hasEaten = false;
    for (GingiCell<B> *c : this->getConnectedCells()) {
      if (c->state == Apoptosis) {
        c->health -= killing;
        this->eatenCounter += killing;
        if (type == Immune)
          this->setColor(1.0, 0.5, 0.5);
        else if (type == Stroma)
          this->setColor(0.5, 0.5, 1.0);
        hasEaten = true;
      } else if (c->state == Necrosis) {
        if (type == Immune) {
          this->setColor(this->getBody().getQuantities()[SIGNAL::INFLAMMATORY],
                         0.0,
                         this->getBody().getQuantities()[SIGNAL::INFLAMMATORY]);
        } else if (type == Stroma) {
          this->setColor(
              0.5 + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5,
              0.5 + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5,
              0.0);
        }
        c->health -= killing;
        this->eatenCounter += killing;
        c->getBody().setConsumption(SIGNAL::INFLAMMATORY, -inflaProd * c->health); // production depends on the
                                                                                  // life of the necrotic cell
        hasEaten = true;
      }
    }
    return hasEaten;
  }

  void updateSignal() {
    // Si une cellule ressent du eat-me d'une cellule en nécrose, elle émet un
    // gradient PI
    if (this->getBody().getQuantities()[SIGNAL::EATME] > 0.0f) {
      this->getBody().setConsumption(SIGNAL::INFLAMMATORY, -inflaProd);
    }

    this->getBody().setConsumption(SIGNAL::INFLAMMATORY, inflaDegrad); // Evaporation
    this->getBody().setConsumption(SIGNAL::RESOLUTIVE, resoDegrad); // Evaporation

    if (this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] > 0.0f)
      this->setColor(
          0.5f + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5f,
          0.5f + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5f,
          0.0);
    else if (type == Immune)
      this->setColor(1.0, 0.5, 0.5);
    else if (type == Stroma)
      this->setColor(0.5, 0.5, 1.0);
  }

  bool shift() {
    bool hasShifted = false;
    // Si une cellule a mangé et ne ressent plus de eat-me
    if (this->eatenCounter > 0.0f &&
        this->getBody().getQuantities()[SIGNAL::EATME] <= 0.001f) {
      // if (this->shiftStep <= 10){
      if (this->shiftStep <= 20) {
        this->getBody().setConsumption(
            SIGNAL::INFLAMMATORY, -(1 - 1 / (1 + 0.1 * exp(-shiftStep + 5))) *
                                      inflaProd); // Production diminue
        this->getBody().setConsumption(SIGNAL::RESOLUTIVE,
                                       -(1 / (1 + 10 * exp(-shiftStep + 5))) *
                                           resoProd); // Production augmente
        this->shiftStep++;
      } else { // if 20 steps (end of the shift), counters reset to 0
        this->eatenCounter = 0.0;
        if (immuneType == Circulatory) {
          this->shiftStep = -5;
        } else {
          this->shiftStep = 0;
        }
      }
      hasShifted = true;
    }
    return hasShifted;
  }

  void disappearanceCirculatory() {
    if (this->immuneType == Circulatory) {
      if (this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] <= 0.0) {
        if (dice(MecaCell::Config::globalRand()) < extinctionProb) {
          this->die();
        }
      }
    }
  }

  void moving() {

    // biased movement according to gradient type
    float movFactorInfla = 1.0;
    float movFactorReso = 1.0;
    float movFactorEatMe = 1.0;

    MecaCell::Vec dir; // direction of movement

    int const neighbouringVoxelsNumber = 26; // 3*3*3 - 1
    double probas[neighbouringVoxelsNumber]; // table that contains the
                                             // probabilities of movement in
                                             // each direction
    MecaCell::Vec coord[neighbouringVoxelsNumber]; // table that contains the coordinates
                                         // of movement in each direction
    int cpt = 0;                         // to browse the tables
    double sumSignals = 0.0; // sum of the 3 signals of all neighboring voxels
    double prob = 0.0;       // sum of the 3 signals of each neighboring voxel

    // current position of the cell on the grid: center of the voxel cube
    MecaCell::Vec posCenter = this->getBody().getGrid()->getIndexFromPosition(this->getPosition());

    // we go through the whole cube (all the voxels), except the center (0,0,0)
    for (int x = -1; x <= 1; x++) {
      for (int y = -1; y <= 1; y++) {
        for (int z = -1; z <= 1; z++) {
          if (x != 0 || y != 0 || z != 0) {
            MecaCell::Vec direction = MecaCell::Vec(x, y, z);
            coord[cpt] = direction; // direction in the table

            // Infla of each voxel
            double valInfla = this->getBody().getGrid()->getMolecule(
                                  posCenter + direction, 0) *
                              movFactorInfla;
            // Reso of each voxel
            double valReso = this->getBody().getGrid()->getMolecule(
                                 posCenter + direction, 1) *
                             movFactorReso;
            // Eat-me of each voxel
            double valEatMe = this->getBody().getGrid()->getMolecule(
                                  posCenter + direction, 2) *
                              movFactorEatMe;

            prob = valInfla + valReso + valEatMe; // sum of the 3 signals
            probas[cpt] =
                prob; // sum of the 3 signals of each voxel on the table
            sumSignals +=
                valInfla + valReso +
                valEatMe; // sum of the 3 signals of all neighboring voxels

            cpt++;
          }
        }
      }
    }

    // Normalization of the probas
    if (sumSignals > 0.0) {
      for (int i = 0; i < neighbouringVoxelsNumber; i++) {
        probas[i] /= sumSignals;
      }
    }

    double unif = dice(MecaCell::Config::globalRand()); // Uniforme (0,1)

    // Cumulative sum on the signals table
    for (int i = 0; i < neighbouringVoxelsNumber - 1; i++) {
      probas[i + 1] += probas[i];
    }

    // We look at the interval in which the uniform pulled is,
    // and we move according to the corresponding vector in the second table
    for (int i = 0; i < neighbouringVoxelsNumber - 1; i++) {
      if (probas[i] < unif <= probas[i + 1]) {
        dir = coord[i + 1] * speed;
      }
    }

    if (dir != MecaCell::Vec(0, 0, 0)) {
      // this->setColor(0.5 +
      // this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5, 0.5 +
      // this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5, 0.0);
      this->body.moveTo(this->getPosition() - dir);
    }

    // if no gradients felt, random movement
    else if (dir == MecaCell::Vec(0, 0, 0)) {
      MecaCell::Vec dir;
      dir = MecaCell::Vec::randomUnit();
      dir *= speed;
      this->body.moveTo(this->getPosition() - dir);
    }
  }

  void eatme() {
    if (!this->isDead()) {
      if (this->state == Apoptosis || this->state == Necrosis) {
        this->getBody().setConsumption(SIGNAL::EATME, -1.0 * this->health); // la cellule en nécrose ou en apoptose émet du
                               // eat-me proportionnellement à son niveau de vie
      }
    }
  }
};

//}

#endif
