#ifndef GINGICELL_H_
#define GINGICELL_H_
#include "../plugin/delaunaymassspringdamper/BodyDelaunayMassSpringDamper.hpp"
#include "../plugin/diffusion/BodyDiffusion.hpp"
#include "Bodies3D.hpp"
#include "GingiScenario.hpp"
#include <mecacell/mecacell.h>

namespace Diffusion {

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
    ImmuneMaker // umber of immunemaker : normal distribution
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
  float eatenCounter; // counter of what the cell has eaten
  int shiftStep; // step for the shift M1/M2


  
  GingiCell(const MecaCell::Vec &v, DiffusionGrid *g)
      : Base(v), dice(0.0f, 1.0f)
  {
    //this->body.moveTo(v);
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

  void init(Type type, nlohmann::json *js) { // Json added
    config = js;
    

      // this->age = 0;
      // this->eatenCounter = 0.0;
      // this->shiftStep = 0;
      // this->speed = (*config)["type"]["speed"];
      // this->divisionProb = (*config)["type"]["divisionProb"];
      // this->deathProb = (*config)["type"]["deathProb"];
      // this->health = (*config)["type"]["health"];
      // this->killing = (*config)["type"]["killing"];
      // this->state = Alive;
      // this->inflaProd = (*config)["type"]["inflaProd"];
      // this->resoProd = (*config)["type"]["resoProd"];
      // this->inflaDegrad = (*config)["type"]["inflaDegrad"];
      // this->resoDegrad = (*config)["type"]["resoDegrad"];
      // this->inflaHealthImpact = (*config)["type"]["inflaHealthImpact"];
      // this->setColor((float)((*config)["type"]["red"]),
      //           (float)((*config)["type"]["green"]),
      //           (float)((*config)["type"]["blue"]));
      // this->type = (Type)(*config)["type"]["type"];
      // this->immuneType = (ImmuneType)(*config)["type"]["immuneType"];

    if (type == Immune) {
      this->age = 0;
      this->eatenCounter = 0.0;
      this->shiftStep = 0;
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
      this->eatenCounter = 0.0;
      this->shiftStep = 0;
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
      this->eatenCounter = 0.0;
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
    
    // Programmed death: apoptosis
    if (dice(MecaCell::Config::globalRand()) < deathProb) { // random number between 0 and 1
      apoptosis();
      return;
    }

    // Impact of inflammation on health
    this->health -= this->inflaHealthImpact * this->getBody().getQuantities()[SIGNAL::INFLAMMATORY];
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

    // Eat me
    eatme(w);

    // Disappearance of Circulatory cells after resolution of the inflammation
    disappearanceCirculatory(avgInfla);


    // Moving
    if (moving() == false) {
      randomMotion(); // if no gradients felt, random movement
    }

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
    GingiCell<B> *daughter = new GingiCell<B>(daughterPostion,this->getBody().getGrid());

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
    // Number of ImmuneMaker cells : normal distribution 
    // You can change the mean and the variance values in j.json file

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
    c->eatenCounter = 0.0;
    c->shiftStep = -5;
    c->immuneType = Circulatory; // Circulatory cells created by ImmuneMaker cells
    c->speed = (float)((*config)["ImmuneCirculatory"]["speed"]); // Circulatory move faster
    c->divisionProb = (float)((*config)["ImmuneCirculatory"]["divisionProb"]);
    c->deathProb = (float)((*config)["ImmuneCirculatory"]["deathProb"]);
    c->inflaHealthImpact = (float)((*config)["ImmuneCirculatory"]["inflaHealthImpact"]);
    c->inflaProd = (float)((*config)["ImmuneCirculatory"]["inflaProd"]); // Circulatory immune cell much more infla than the resident one
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

      if (dice(MecaCell::Config::globalRand()) < divisionProb * health) {
        if (!this->isDead()) {
          GingiCell<B> *daughter = divide();
          w.addCell(daughter);
          b = true;
        }
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
      this->eatenCounter +=killing;
      b = true;

    } else if (c->state == Necrosis) {
      if (type == Immune)
        this->setColor(this->getBody().getQuantities()[SIGNAL::INFLAMMATORY], 0.0, this->getBody().getQuantities()[SIGNAL::INFLAMMATORY]);
      else if (type == Stroma)
        this->setColor(0.5 + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5, 0.5 + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5, 0.0);
      c->getBody().setConsumption(SIGNAL::INFLAMMATORY, -inflaProd * c->health); // production depends on the life of the necrotic cell
      c->health -= killing;
      //std::cerr << c->health << std::endl;
      this->eatenCounter +=killing;
    }
    return b;
  }



  template <class W> void updateSignal(double *avgInfla, W &w) {

     for (auto *c : this->getConnectedCells()) {
      *avgInfla = max(*avgInfla + c->getBody().getQuantities()[SIGNAL::INFLAMMATORY] / this->getConnectedCells().size() -
                          c->getBody().getQuantities()[SIGNAL::RESOLUTIVE] / this->getConnectedCells().size(),(double)0);
     }

    GingiCell<B> *closestNecroCell = nullptr;
    for (GingiCell<B> *cell : w.cells) {
      if (!cell->isDead() && cell != this) {
        if (cell->state == Necrosis)  {
          closestNecroCell = cell;
          }
        }
      }

    // Si une cellule ressent du eat-me d'une cellule en n??crose, elle ??met un gradient PI
    if (closestNecroCell) {
      if (this->getBody().getQuantities()[SIGNAL::EATME] > 0.0f) {
        this->getBody().setConsumption(SIGNAL::INFLAMMATORY, -inflaProd);
      }

    // Si une cellule a mang?? et ne ressent plus de eat-me 
    if (this->eatenCounter > 0.0f && this->getBody().getQuantities()[SIGNAL::EATME] <= 0.001f) {
      //if (this->shiftStep <= 10){
      if (this->shiftStep <= 20){
        this->getBody().setConsumption(SIGNAL::INFLAMMATORY, -(1-1/(1 + 0.1 * exp(-shiftStep+5)))); // * this->getBody().getQuantities()[SIGNAL::INFLAMMATORY]); //Production diminue
        this->getBody().setConsumption(SIGNAL::RESOLUTIVE, -(1/(1+10 * exp(-shiftStep + 5)))); // * this->getBody().getQuantities()[SIGNAL::RESOLUTIVE]); // Production augmente
        this->shiftStep++;
      }
      else { // if 20 steps (end of the shift), counters reset to 0
        this->eatenCounter = 0.0;
        if (immuneType == Circulatory) {
          this->shiftStep = -5;
        }
        else {
          this->shiftStep = 0;
        }        
      }

    }

    this->getBody().setConsumption(SIGNAL::INFLAMMATORY, inflaDegrad); // Evaporation
    this->getBody().setConsumption(SIGNAL::RESOLUTIVE, resoDegrad); // Evaporation

    if (this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] > 0.0f)
      this->setColor(0.5f + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5f, 0.5f + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5f, 0.0);
    else if (type == Immune)
      this->setColor(1.0, 0.5, 0.5);
    else if (type == Stroma)
      this->setColor(0.5, 0.5, 1.0);
  }

}


  void disappearanceCirculatory(double avgInfla) {
    if (avgInfla <= 0) {
      if (this->immuneType == Circulatory && dice(MecaCell::Config::globalRand()) < extinctionProb) {
        this->die();
      }
    }
  }


  bool moving() {
    bool b = false;

    // biased movement according to gradient type
    float movFactorInfla = 1.0;
    float movFactorReso = 1.0;
    float movFactorEatMe = 1.0;
    
    MecaCell::Vec dir; // direction of movement

    int const neighbouringVoxelsNumber = 26; // 3*3*3 - 1
    double probas[neighbouringVoxelsNumber]; // table that contains the probabilities of movement in each direction
    MecaCell::Vec coord[neighbouringVoxelsNumber]; // table that contains the coordinates of movement in each direction
    int cpt = 0; // to browse the tables
    double sumSignals = 0.0; // sum of the 3 signals of all neighboring voxels
    double prob = 0.0; // sum of the 3 signals of each neighboring voxel


    // current position of the cell on the grid: center of the voxel cube
    MecaCell::Vec posCenter = this->getBody().getGrid()->getIndexFromPosition(this->getPosition());

    // we go through the whole cube (all the voxels), except the center (0,0,0)
    for (int x = -1 ; x<=1 ; x++) {
      for (int y = -1 ; y<=1 ; y++) {
        for (int z = -1 ; z<=1 ; z++) {
          if (x!=0 || y!=0 || z!=0) {
            MecaCell::Vec direction = MecaCell::Vec(x, y, z);
            coord[cpt] = direction; // direction in the table

            // Infla of each voxel
            double valInfla = this->getBody().getGrid()->getMolecule(posCenter+direction,0) * movFactorInfla;
            // Reso of each voxel
            double valReso = this->getBody().getGrid()->getMolecule(posCenter+direction,1) * movFactorReso;
            // Eat-me of each voxel
            double valEatMe = this->getBody().getGrid()->getMolecule(posCenter+direction,2) * movFactorEatMe;

            prob = valInfla + valReso + valEatMe; // sum of the 3 signals
            probas[cpt] = prob; // sum of the 3 signals of each voxel on the table
            sumSignals += valInfla + valReso + valEatMe; // sum of the 3 signals of all neighboring voxels

            cpt++;

          }
        }
      }
    }

    // Normalization of the probas
    if (sumSignals > 0.0) {
      for (int i = 0 ; i < neighbouringVoxelsNumber ; i++) {
        probas[i] /= sumSignals;
      }
    }
    

    double unif = dice(MecaCell::Config::globalRand()); //Uniforme (0,1)

    // Cumulative sum on the signals table
    for (int i = 0 ; i < neighbouringVoxelsNumber - 1 ; i++) {
      probas[i+1] += probas[i];
    }
    
  
    // We look at the interval in which the uniform pulled is,
    // and we move according to the corresponding vector in the second table
    for (int i = 0 ; i < neighbouringVoxelsNumber - 1 ; i++) {
      if (probas[i] < unif <= probas[i+1]) {
        dir = coord[i+1] * speed;
      }
    }
    
    

    if (dir != MecaCell::Vec(0, 0, 0)) {
      //this->setColor(0.5 + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5, 0.5 + this->getBody().getQuantities()[SIGNAL::INFLAMMATORY] * 0.5, 0.0);
      this->body.moveTo(this->getPosition() - dir);
      b = true;
    }

    
    return b;
    
  }


  void randomMotion() {
    MecaCell::Vec dir;
    dir = MecaCell::Vec::randomUnit();
    dir *= speed;
    this->body.moveTo(this->getPosition() - dir);
  }



  template <class W> void eatme(W &w) {
    //GingiCell<B> *closestNecroCell = nullptr;
    for (GingiCell<B> *c : w.cells) {
      if (!c->isDead() && c != this) {
        if (c->state == Apoptosis || c->state == Necrosis) {
          c->getBody().setConsumption(SIGNAL::EATME, -1.0 * c->health); // la cellule en n??crose ou en apoptose ??met du eat-me proportionnellement ?? son niveau de vie
          }
        
        }
      }
    }


  void initCells(Type type, nlohmann::json *js) {
    config = js;
    
    this->age = 0;
    this->eatenCounter = 0.0;
    this->shiftStep = 0;
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
  }


};

}


#endif
