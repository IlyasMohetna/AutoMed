import { useState, useEffect } from "react";
import { Activity, Plus, AlertCircle, ServerCrash } from "lucide-react";
import ApiService from "./services/ApiService";
import SimulationCard from "./components/SimulationCard";
import SimulationForm from "./components/SimulationForm";
import SimulationLargeView from "./components/SimulationLargeView";
import { Dialog, DialogContent } from "@/components/ui/dialog";
import { Button } from "@/components/ui/button";
import { Badge } from "@/components/ui/badge";
import { Card, CardContent } from "@/components/ui/card";
import "./index.css";

function App() {
  const [connected, setConnected] = useState(false);
  const [serverInfo, setServerInfo] = useState(null);
  const [simulations, setSimulations] = useState([]);
  const [simulationStatuses, setSimulationStatuses] = useState({});
  const [showForm, setShowForm] = useState(false);
  const [largeViewSimulation, setLargeViewSimulation] = useState(null);

  useEffect(() => {
    checkHealth();
    const healthInterval = setInterval(checkHealth, 5000);
    return () => clearInterval(healthInterval);
  }, []);

  useEffect(() => {
    if (connected) {
      loadSimulations();
      const simInterval = setInterval(loadSimulations, 3000); // Refresh toutes les 3 secondes
      return () => clearInterval(simInterval);
    }
  }, [connected]);

  useEffect(() => {
    if (simulations.length > 0) {
      loadAllStatuses();
    }
  }, [simulations]);

  const checkHealth = async () => {
    try {
      await ApiService.healthCheck();
      setConnected(true);
      if (!serverInfo) {
        const info = await ApiService.getInfo();
        setServerInfo(info);
      }
    } catch (error) {
      setConnected(false);
    }
  };

  const loadSimulations = async () => {
    try {
      const data = await ApiService.listSimulations();
      setSimulations(data.simulations || []);
    } catch (error) {
      console.error("Error loading simulations:", error);
    }
  };

  const loadAllStatuses = async () => {
    const statuses = {};
    for (const simId of simulations) {
      try {
        const status = await ApiService.getSimulationStatus(simId);
        statuses[simId] = status;
      } catch (error) {
        console.error(`Error loading status for sim ${simId}:`, error);
      }
    }
    setSimulationStatuses(statuses);
  };

  const handleCreateSimulation = async (config) => {
    try {
      const result = await ApiService.createSimulation(config);
      setShowForm(false);
      loadSimulations();
    } catch (error) {
      console.error("Error creating simulation:", error);
    }
  };

  const handleStartSimulation = async (simId) => {
    try {
      await ApiService.startSimulation(simId);
      setLargeViewSimulation(simId);
      loadSimulations();
    } catch (error) {
      console.error("Error starting simulation:", error);
    }
  };

  const handlePauseSimulation = async (simId) => {
    try {
      await ApiService.pauseSimulation(simId);
      loadSimulations();
    } catch (error) {
      console.error("Error pausing simulation:", error);
    }
  };

  const handleResumeSimulation = async (simId) => {
    try {
      await ApiService.resumeSimulation(simId);
      loadSimulations();
    } catch (error) {
      console.error("Error resuming simulation:", error);
    }
  };

  const handleStopSimulation = async (simId) => {
    try {
      await ApiService.stopSimulation(simId);
      setLargeViewSimulation(null);
      loadSimulations();
    } catch (error) {
      console.error("Error stopping simulation:", error);
    }
  };

  const handleDeleteSimulation = async (simId) => {
    if (!confirm(`Êtes-vous sûr de vouloir supprimer la simulation #${simId} ?`)) {
      return;
    }
    try {
      await ApiService.deleteSimulation(simId);
      loadSimulations();
    } catch (error) {
      console.error("Error deleting simulation:", error);
    }
  };

  if (largeViewSimulation) {
    return (
      <SimulationLargeView
        simulationId={largeViewSimulation}
        onClose={() => setLargeViewSimulation(null)}
        onPause={handlePauseSimulation}
        onResume={handleResumeSimulation}
        onStop={handleStopSimulation}
      />
    );
  }

  return (
    <div className="min-h-screen bg-background">
      {/* Modern Header */}
      <header className="sticky top-0 z-40 border-b bg-background/95 backdrop-blur supports-[backdrop-filter]:bg-background/60">
        <div className="container flex h-16 items-center justify-between px-4">
          <div className="flex items-center gap-3">
            <Activity className="h-6 w-6 text-primary" />
            <div>
              <h1 className="text-xl font-bold">AutoMed</h1>
              <p className="text-xs text-muted-foreground">Bloc Opératoire</p>
            </div>
          </div>
          
          <div className="flex items-center gap-4">
            <div className="flex items-center gap-2">
              <div className={`h-2 w-2 rounded-full ${connected ? "bg-emerald-500 animate-pulse" : "bg-destructive"}`} />
              <span className="text-sm font-medium">
                {connected ? "Connecté" : "Déconnecté"}
              </span>
            </div>
            {serverInfo && (
              <Badge variant="outline" className="hidden md:flex">
                v{serverInfo.version}
              </Badge>
            )}
          </div>
        </div>
      </header>

      {/* Main Content */}
      <main className="container px-4 py-8">
        {!connected ? (
          <Card className="border-destructive">
            <CardContent className="flex items-center gap-4 p-6">
              <ServerCrash className="h-12 w-12 text-destructive" />
              <div>
                <h3 className="font-semibold text-lg">Serveur Non Connecté</h3>
                <p className="text-sm text-muted-foreground">
                  En attente de connexion au backend AutoMed...
                </p>
              </div>
            </CardContent>
          </Card>
        ) : (
          <>
            {/* Header Section */}
            <div className="flex items-center justify-between mb-8">
              <div>
                <h2 className="text-3xl font-bold tracking-tight">Simulations</h2>
                <p className="text-muted-foreground mt-1">
                  {simulations.length} simulation{simulations.length !== 1 ? "s" : ""} active{simulations.length !== 1 ? "s" : ""}
                </p>
              </div>
              <Button onClick={() => setShowForm(true)} size="lg">
                <Plus className="h-5 w-5 mr-2" />
                Nouvelle Simulation
              </Button>
            </div>

            {/* Empty State */}
            {simulations.length === 0 ? (
              <Card className="border-dashed">
                <CardContent className="flex flex-col items-center justify-center p-12 text-center">
                  <AlertCircle className="h-16 w-16 text-muted-foreground mb-4" />
                  <h3 className="text-xl font-semibold mb-2">Aucune Simulation</h3>
                  <p className="text-muted-foreground mb-6 max-w-sm">
                    Commencez par créer votre première simulation pour surveiller les blocs opératoires
                  </p>
                  <Button onClick={() => setShowForm(true)}>
                    <Plus className="h-4 w-4 mr-2" />
                    Créer une Simulation
                  </Button>
                </CardContent>
              </Card>
            ) : (
              <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
                {simulations.map((simId) => (
                  <SimulationCard
                    key={simId}
                    simulation={simId}
                    status={simulationStatuses[simId]}
                    onStart={handleStartSimulation}
                    onPause={handlePauseSimulation}
                    onResume={handleResumeSimulation}
                    onStop={handleStopSimulation}
                    onDelete={handleDeleteSimulation}
                    onViewLarge={setLargeViewSimulation}
                  />
                ))}
              </div>
            )}
          </>
        )}
      </main>

      {/* Create Simulation Dialog */}
      <Dialog open={showForm} onOpenChange={setShowForm}>
        <DialogContent className="max-w-4xl max-h-[90vh] overflow-y-auto">
          <SimulationForm
            onSimulationCreated={handleCreateSimulation}
            onCancel={() => setShowForm(false)}
          />
        </DialogContent>
      </Dialog>
    </div>
  );
}

export default App;
