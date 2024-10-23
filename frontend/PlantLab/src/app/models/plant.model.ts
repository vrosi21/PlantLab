export interface Plant {
  type: string;
  name: string;
  scientificName: string;
  family: string;
  genus: string;
  kingdom: string;
  order: string;
  tribe: string;
  recommendedMoistureLevel: number;
  id: number;
  currentMoistureLevel?: number;
}
