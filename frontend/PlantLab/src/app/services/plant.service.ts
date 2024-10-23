import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable } from 'rxjs';
import { Plant } from '../models/plant.model';
import { environment } from '../../environments/environment';

@Injectable({
  providedIn: 'root',
})
export class PlantService {
  private apiUrl = environment.apiUrl;

  constructor(private http: HttpClient) {}

  getPlants(): Observable<Plant[]> {
    return this.http.get<Plant[]>(`${this.apiUrl}/plants`);
  }

  waterPlant(id: number): Observable<any> {
    return this.http.post(`${this.apiUrl}/waterplant/${id}`, {});
  }

  addPlant(plant: Plant): Observable<any> {
    const headers = new HttpHeaders({ 'Content-Type': 'application/json' });
    return this.http.post(`${this.apiUrl}/addplant`, plant, { headers });
  }
}
