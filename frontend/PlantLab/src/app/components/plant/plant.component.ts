import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { PlantService } from '../../services/plant.service';
import { Plant } from '../../models/plant.model';
import { HttpClientModule } from '@angular/common/http';

@Component({
  selector: 'app-plant',
  standalone: true,
  imports: [CommonModule, HttpClientModule],
  templateUrl: './plant.component.html',
  styleUrls: ['./plant.component.css'],
})
export class PlantComponent implements OnInit {
  plants: Plant[] = [];

  private temporaryPlants: Plant[] = [
    {
      type: 'Tree',
      name: 'Fikret',
      scientificName: 'Ficus',
      family: 'Moraceae',
      genus: 'Ficus; L.',
      kingdom: 'Plantae',
      order: 'Rosales',
      tribe: 'Ficeae; Dumort.',
      recommendedMoistureLevel: 30,
      id: 1,
      currentMoistureLevel: 45,
    },
    {
      type: 'Herb',
      name: 'Basil',
      scientificName: 'Ocimum basilicum',
      family: 'Lamiaceae',
      genus: 'Ocimum',
      kingdom: 'Plantae',
      order: 'Lamiales',
      tribe: 'Ocimeae',
      recommendedMoistureLevel: 50,
      id: 2,
      currentMoistureLevel: 60,
    },
    {
      type: 'Shrub',
      name: 'Rosemary',
      scientificName: 'Salvia rosmarinus',
      family: 'Lamiaceae',
      genus: 'Salvia',
      kingdom: 'Plantae',
      order: 'Lamiales',
      tribe: 'Mentheae',
      recommendedMoistureLevel: 40,
      id: 3,
      currentMoistureLevel: 35,
    },
  ];

  constructor(private plantService: PlantService) {}

  ngOnInit(): void {
    // this.loadPlants();  // Uncomment this to fetch from service
    this.plants = this.temporaryPlants; // Use temporary plants data
  }

  loadPlants(): void {
    this.plantService.getPlants().subscribe(
      (plants: Plant[]) => {
        this.plants = plants;
      },
      (error) => {
        console.error('Error fetching plants:', error);
      }
    );
  }

  waterPlant(id: number): void {
    this.plantService.waterPlant(id).subscribe(() => {
      console.log(`Watered plant with ID ${id}`);
    });
  }
}
