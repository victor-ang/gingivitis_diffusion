#ifndef _GINGISCENARIO_HPP_
#define _GINGISCENARIO_HPP_
#include "Bodies3D.hpp"
#include "GingiCell.hpp"
#include <fstream>
#include <mecacell/mecacell.h>
#include <sstream>
#include <string>

class GingiScenario {

public:
  using Cell = GingiCell<Bodies3D>;
  using World = MecaCell::World<Cell>;

  World w;
  nlohmann::json js;

  GingiScenario() {}

  static double BOX_HALF_SIZE;

  inline World &getWorld() { return w; }

  void init() {

    js = loadJsonConfig("../j.json");
    Molecule infla(400.0, 0.0, 1.0, 0.1);
    Molecule reso(400.0, 0.0, 1.0, 0.1);
    Molecule eatme(600.0, 0.0, 1.0, 0.1);

    w.cellPlugin.diffusionPlugin.addMolecule(infla);
    w.cellPlugin.diffusionPlugin.addMolecule(reso);
    w.cellPlugin.diffusionPlugin.addMolecule(eatme);

    w.setDt(1);
    // Add cells
    //		for (int i = -5; i <= 5; i++) {
    //			for (int j=-5; j <= 5; j++) {
    //				for (int k=-5; k <= 5; k++) {
    //					Cell* c = new Cell(MecaCell::Vec(i *
    //MecaCell::Config::DEFAULT_CELL_RADIUS * 1.75f, 													 j *
    //MecaCell::Config::DEFAULT_CELL_RADIUS * 1.75f, 													 k *
    //MecaCell::Config::DEFAULT_CELL_RADIUS * 1.75f)); 					w.addCell(c);
    //				}
    //			}
    //		}
    // std::normal_distribution<double>
    // nDist(-5.0f*MecaCell::Config::DEFAULT_CELL_RADIUS * 1.75f, 5.0f *
    // MecaCell::Config::DEFAULT_CELL_RADIUS * 1.75f);
    std::uniform_real_distribution<double> nDist(-BOX_HALF_SIZE, BOX_HALF_SIZE);

    // Immune cells
    int nCells = js["nbCells"]; // Number of cells at the beginning
    float ratio = js["ratioImmuneStromal"]; // Ratio Immune/Stromale
    int k = 0;
    Cell *c;
    while (k < nCells * ratio) {
      k++;
      MecaCell::Vec pos(nDist(MecaCell::Config::globalRand()),
                        nDist(MecaCell::Config::globalRand()),
                        nDist(MecaCell::Config::globalRand()));
      c = new Cell(pos);
      c->init(Cell::Immune, &js);
      w.addCell(c);
    }
    while (k < nCells) {
      k++;
      MecaCell::Vec pos(nDist(MecaCell::Config::globalRand()),
                        nDist(MecaCell::Config::globalRand()),
                        nDist(MecaCell::Config::globalRand()));
      c = new Cell(pos);
      c->init(Cell::Stroma, &js); // ajout du json
      w.addCell(c);
    }

    // Number of ImmuneMaker cells
    std::random_device rd; // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(js["lowValue"],
                                        js["highValue"]); // Uniform(a,b))
    int nbImmuneMaker = dis(gen);                         //  nbImmuneMaker
    for (int k = 0; k < nbImmuneMaker; k++) {
      MecaCell::Vec pos(nDist(MecaCell::Config::globalRand()),
                        nDist(MecaCell::Config::globalRand()),
                        nDist(MecaCell::Config::globalRand()));
      c = new Cell(pos);
      c->init(Cell::ImmuneMaker, &js);
      w.addCell(c);
    }

    w.addNewCells();
  }

  void loop() {
    if (!stop()) {

      if (w.getNbUpdates() % 1 == 0) {

        // Création de fichiers (1 par step) qui contiennent les infos des cellules

        /* std::ofstream myfile;
        std::string fileName = "../CSV/csvStep"; // file name
        std::string extension = ".csv"; // extension
        std::stringstream fichier; // fichier à ouvrir
        fichier << fileName << w.getNbUpdates() << extension; // nouveau nom
        myfile.open(fichier.str().c_str(),std::ofstream::app); */

        int nbAliveStroma = 0;
        int nbApopStroma = 0;
        int nbAliveImmune = 0;
        int nbApopImmune = 0;
        int nbNecroStroma = 0;
        int nbAliveImmuneResident = 0;
        int nbAliveImmuneCirculatory = 0;
        int nbNecroImmune = 0;
        int nbImmuneMaker = 0;
        double avgInfla = 0;
        for (Cell *c : w.cells) {

          // Ecriture dans le fichier csv

          // myfile << c->type << "," << c->health << "," << c->infla << "," <<
          // c->reso << "," << c->getPosition() << std::endl;

          if (c->type == Cell::Immune) {
            if (c->state == Cell::Alive) {
              nbAliveImmune++;
              if (c->immuneType == Cell::Resident)
                nbAliveImmuneResident++;
              else if (c->immuneType == Cell::Circulatory)
                nbAliveImmuneCirculatory++;
            } else if (c->state == Cell::Apoptosis) {
              nbApopImmune++;
            } else if (c->state == Cell::Necrosis) {
              nbNecroImmune++;
            }
          } else if (c->type == Cell::Stroma) {
            if (c->state == Cell::Alive)
              nbAliveStroma++;
            else if (c->state == Cell::Apoptosis)
              nbApopStroma++;
            else if (c->state == Cell::Necrosis)
              nbNecroStroma++;
          } else if (c->type == Cell::ImmuneMaker) {
            nbImmuneMaker++;
          }
          avgInfla += c->getBody().getQuantities()[SIGNAL::INFLAMMATORY];
        }
        avgInfla /= w.cells.size();

        cerr << w.getNbUpdates() << "\t" << w.cells.size() << "\t"
             << nbAliveStroma << "\t" << nbApopStroma << "\t" << nbNecroStroma
             << "\t" << nbAliveImmune << "\t" << nbAliveImmuneResident << "\t"
             << nbAliveImmuneCirculatory << "\t" << nbApopImmune << "\t"
             << nbNecroImmune << "\t" << nbImmuneMaker << "\t" << avgInfla
             << endl;
        writeToCSV(w.getNbUpdates(), w.cells.size(), nbAliveStroma,
                   nbApopStroma, nbNecroStroma, nbAliveImmune,
                   nbAliveImmuneResident, nbAliveImmuneCirculatory,
                   nbApopImmune, nbNecroImmune, nbImmuneMaker, avgInfla);

        // myfile.close(); // Fermeture du csv contenant toutes les cellules
      }

      w.update();
    }
  }

  bool stop() { return false; }

  // Import json file
  nlohmann::json loadJsonConfig(std::string fileName) {
    nlohmann::json config;
    std::ifstream i(fileName, std::ifstream::in);
    i >> config;
    i.close();
    return (config);
  }

  // Function to export values step by step (1row-> average of cells) to csv
  void writeToCSV(int nbIter, int nbCells, int nbAliveStroma, int nbApopStroma,
                  int nbNecroStroma, int nbAliveImmune,
                  int nbAliveImmuneResident, int nbAliveImmuneCirculatory,
                  int nbApopImmune, int nbNecroImmune, int nbImmuneMaker,
                  float avgInfla) {
    std::ofstream myfile;

    if (nbIter == 0) {
      myfile.open("../data.csv", std::ofstream::trunc);
      myfile << "Nombre d'itérations"
             << ","
             << "Nombre de cellules"
             << ","
             << "Nombre de cellules stromales vivantes"
             << ","
             << "Nombre de cellules stromales en apoptose"
             << ","
             << "Nombre de cellules stromales en nécrose"
             << ","
             << "Nombre de cellules immunitaires vivantes"
             << ","
             << "Nombre de cellules immunitaires résidentes vivantes"
             << ","
             << "Nombre de vellules immunitaires circulantes vivantes"
             << ","
             << "Nombre de cellules immunitaires en apoptose"
             << ","
             << "Nombre de cellules immunitaires en nécrose"
             << ","
             << "Nombre d'ImmuneMaker"
             << ","
             << "Moyenne de l'inflammation" << std::endl;
      myfile.close();
    }
    myfile.open("../data.csv", std::ofstream::app);
    myfile << nbIter << "," << nbCells << "," << nbAliveStroma << ","
           << nbApopStroma << "," << nbNecroStroma << "," << nbAliveImmune
           << "," << nbAliveImmuneResident << "," << nbAliveImmuneCirculatory
           << "," << nbApopImmune << "," << nbNecroImmune << ","
           << nbImmuneMaker << "," << avgInfla << std::endl;
    myfile.close();
  }
};

double GingiScenario::BOX_HALF_SIZE = 250.0;

#endif // _GINGISCENARIO_H
