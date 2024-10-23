import { Component } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { PlantComponent } from './components/plant/plant.component';
import { HttpClientModule } from '@angular/common/http';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [RouterOutlet, PlantComponent, HttpClientModule],
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css'],
})
export class AppComponent {
  title = 'PlantLab';
}
